<?php
function employer_sendresumerequest_submit($form, &$form_state)
{
     global $user,$base_url;
    /*Decrypting the data using configration key*/
    $decrypting_key=variable_get('IIUM_Encryption_Key', NULL);
    $student_id=(int)decrypt($form_state['values']['student_id'],$decrypting_key);

  
    $employer_interviewrequest = db_insert('student_resume_request')
     ->fields(array(
        'employer_id' => trim($user->uid),
        'student_id' => trim($student_id),
        'your_email_id' => trim($form_state['values']['email_id']),
        'subject'=> trim($form_state['values']['subject']),
        'message' => trim($form_state['values']['message_all']),
        'federal_title' => trim($form_state['values']['federal_title']),
        'p_title' => trim($form_state['values']['title']), 
        'contactperson_fname' => trim($form_state['values']['fname']),
        'contactperson_lname' => trim($form_state['values']['lname']),  
        'contactperson_designation' => trim($form_state['values']['designation']),
        'contactperson_company' => trim($form_state['values']['company']),
        'contactperson_call' => trim($form_state['values']['contact_call']),
        'contactperson_email' => trim($form_state['values']['email']),
        'status'=> 2,
        'created_date' => trim(date('Y-m-d H:i:s')),
  ))
  ->execute();
    
    
   $studentname=studentname($student_id); 
   $path=$base_url.'/'.drupal_get_path('theme', 'bootstrap_business');    
   $body='<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>IIUM Email</title>
</head>

<body style="font-family:Arial, Helvetica, sans-serif; font-size:14px; line-height:30px; margin:0; padding:0;">
<table width="600" border="0" cellspacing="0" cellpadding="0" style="margin:0 auto; border: 1px solid #ccc; padding:0 0 15px 0;">
  <tr>
    <td style="background:url('.$path.'/images/header-bg.jpg) repeat-x left top;border-bottom: 5px solid #9c2b3d; padding: 15px 10px; text-align: center;"><img src="'.$path.'/images/logo.png" width="228" height="83" /></td>
  </tr>
  <tr>
    <td style="padding: 0px 15px;"><h1 style="padding: 0px 0 5px; margin: 10px 0px 0; font-family: arial; font-weight: 600; color: #663398; font-size: 20px; border-bottom:1px solid #ccc;">Resume Request -<span style="font-size:15px; color:#000; font-weight:normal;"> This Request Expires in 24 hours of the delivery</span></h1></td>
  </tr>
  <tr>
    <td><table width="600" border="0" cellspacing="0" cellpadding="0" style="padding:10px 15px;">
  <tr>
    <td valign="top"><b>Message:</b></td>
    <td><p style="margin: 0px; line-height: 24px;">Dear '.$studentname[0]->name.',<br>'.$form_state['values']['message_all'].'</p></td>
  </tr>
  <tr>
    <td colspan="2"><h2 style="color:#9c2b3d; padding: 0px 0px 5px; border-bottom: 1px solid #ccc; margin: 8px 0px 7px; font-weight: 600; font-size: 17px; line-height: 22px;">Contact Details:</h2></td>
  </tr>
  <tr>
    <td><b>Name:</b></td>
    <td>'.$form_state['values']['fname'].' '.$form_state['values']['lname'].'</td>
  </tr>
  <tr>
    <td><b>Designation:</b></td>
    <td>'.$form_state['values']['designation'].'</td>
  </tr>
  <tr>
    <td><b>Company:</b></td>
    <td>'.$form_state['values']['company'].'</td>
  </tr>
  <tr>
    <td><b>Call:</b></td>
    <td>'.$form_state['values']['contact_call'].'</td>
  </tr>
  <tr>
    <td><b>Email:</b></td>
    <td>'.$form_state['values']['email'].'</td>
  </tr>
</table>
</td>
  </tr>
</table>
</body>
</html>';  
    
    
       $my_module = 'foo';
       $my_mail_token = 'bar';
       $from =$form_state['values']['email_id'];
      
$message = array(
'to' => $studentname[0]->alt_email,
'subject' => $form_state['values']['subject'],
'body' => $body,
'headers' => array(
'From' => $form_state['values']['email_id'],
'MIME-Version' => '1.0',
'Content-Type' => 'text/html;charset=utf-8',),
       );
     
    //Drupal mail function core
    $system = drupal_mail_system($my_module, $my_mail_token);
       
    $user_status_history=user_status_history($user->uid,$student_id,0,2,'employer');
    //audit log function
    audit_log('sent resume request', 'User ID:'.$student_id); 
   if ($system->mail($message)) 
   {
    drupal_set_message('Your Resume request for  visibility of candidate resume is sucessfully sent, this request will get expired in next 24 hours if candidate do not respond to it.', 'status');
    echo '<script type="text/javascript">  setTimeout(function(){window.close();  window.opener.location.reload();  }, 200); </script>';
    exit; 
   }
   else{
    drupal_set_message('Mail error', 'error');
   }

}
/*Send resume request function*/
function employer_sendresumerequest($form, &$form_state,$student_id)
{   

     global $user;
     if($user->uid==0)
      drupal_goto('user');
         
    drupal_add_js(drupal_get_path('theme', 'bootstrap_business') .'/js/jquery.validate.js');  
    drupal_add_js(drupal_get_path('theme', 'bootstrap_business') .'/js/jqueryformvalidation.js');
    drupal_add_js(drupal_get_path('module', 'employer') .'/js/sendresumerequest_custom_validations.js');
    drupal_add_js("function printpage(){
            window.print();
         }
		 function closewindow(){
		    window.close(); 
		 }
		 ",'inline');
    
     /*Fetch Employer contact details*/
      $employer_details_query = db_select('employer_registration','c');
      $employer_details_query->innerjoin('users', 'u', 'u.uid = c.uid');
      $employer_details_query->fields('c',array('federal_title','p_title','company_organization','firstname','lastname','designation','telephone'));
      $employer_details_query->fields('u',array('mail')); 
      $employer_details_query->condition('c.uid',$user->uid, '='); 
      $employer_details=$employer_details_query->execute()
                                               ->fetchAll();
    
    $form = array();
    $form['student_id'] = array(
    '#type' => 'hidden', 
    '#value' => $student_id,
    );
    $form['send_interview'] = array(
     '#type' => 'item',
     '#markup' => '<h1>Send Resume Request</h1>',  
    );
    $form['send_status'] = array(
     '#type' => 'item',
     '#markup' => '<div class="note">Note:This Request Expires in 24 hours of the delivery</div>',  
    );
    $form['email_id'] = array(
    '#type' => 'textfield', 
    '#title' => 'Your Email ID',
    '#required' => TRUE,
    '#default_value' => (isset($employer_details[0]->mail)?$employer_details[0]->mail:''),
    );
    $form['subject'] = array(
    '#type' => 'textfield', 
    '#title' => 'Subject',
    '#required' => TRUE,
    '#attributes' => array('placeholder' => 'Please provide the permission to view your resume'),
    );
    $form['message'] = array(
     '#type' => 'item',
     '#markup' => '<h2>Message</h2>',  
    );
    $form['message_all'] = array(
    '#type' => 'textarea', 
    '#id' => 'message_all', 
    '#title' => 'Message',
    '#required' => TRUE,
    '#attributes' => array('placeholder' => 'It looks like your profile data matches with requirment we have posted for our new job post,could you please provide us the access to review your Resume.access before 24 hours of the notification delivery.Else, it is considered as expired '),
    );
    $form['request_sent_by'] = array(
     '#type' => 'item',
     '#markup' => '<h2>Request sent by:</h2>',  
    ); 
    $form['address'] = array(
        '#type' => 'fieldset',      
    ); 
    $form['address']['federal_title'] = array(
        '#title' => 'Federal/State title',
        '#type' => 'select',
        '#options' => array('' => 'Select') + comp_titles(),
        '#required' => TRUE,
        '#default_value' => (isset($employer_details[0]->federal_title)?$employer_details[0]->federal_title:''),
    ); 
    $form['address']['title'] = array(
        '#type' => 'select',
        '#id' => 'title',
        '#title' => 'Title',
        '#options' => array(
            '' => 'Select',
            'Dr' => 'Dr.',
            'Mr' => 'Mr.',
            'Mrs' => 'Mrs.',
            'Ms' => 'Ms.',
        ),
        '#default_value' => (isset($employer_details[0]->p_title)?$employer_details[0]->p_title:''),
        '#required' => TRUE, //make this field required
    );
    $form['address']['fname'] = array(
      '#type' => 'textfield', 
      '#id' => 'fname', 
      '#title' => 'Firstname',
      '#required' => TRUE, //make this field required
      '#maxlength' => 20, 
      '#default_value' => (isset($employer_details[0]->firstname)?$employer_details[0]->firstname:''),
    );
    $form['address']['lname'] = array(
      '#type' => 'textfield', 
      '#id' => 'lname', 
      '#title' => 'Lastname',
      '#required' => TRUE, //make this field required
      '#maxlength' => 20, 
      '#default_value' => (isset($employer_details[0]->lastname)?$employer_details[0]->lastname:''),
    );
   $form['address']['designation'] = array(
    '#type' => 'textfield', 
    '#id' => 'designation', 
    '#title' => 'Designation',
    '#required' => TRUE, //make this field required
    '#maxlength' => 30, 
    '#default_value' => (isset($employer_details[0]->designation)?$employer_details[0]->designation:''),
   );
  $form['address']['company'] = array(
    '#type' => 'textfield', 
    '#id' => 'company', 
    '#title' => 'Company Name',
    '#maxlength' => 70,
    '#required' => TRUE,
    '#default_value' => (isset($employer_details[0]->company_organization)?$employer_details[0]->company_organization:''),
   );
  $form['address']['contact_call'] = array(
    '#type' => 'textfield', 
    '#id' => 'contact_call', 
    '#title' => 'Call',
    '#maxlength' => 30, 
    '#default_value' => (isset($employer_details[0]->telephone)?$employer_details[0]->telephone:''),
  );
 $form['address']['email'] = array(
    '#type' => 'textfield', 
    '#id' => 'email', 
    '#title' => 'Email',
    '#maxlength' => 50, 
    '#default_value' => (isset($employer_details[0]->mail)?$employer_details[0]->mail:''),
  );
 $form['submit'] = array(
    '#type' => 'submit', 
    '#id' => 'submit', 
    '#value' => 'submit',
    '#submit' => array('employer_sendresumerequest_submit'),
  );
  $form['button'] = array(
    '#type' => 'button', 
    '#id' => 'button', 
    '#value' => 'Close',
    '#attributes' => array("onclick" => "closewindow()"),
  );
    return $form;
    
}
/**
 * Get Student name in student details table based on employer table
 */
function studentname($student_id)
{
  $studentname = db_select('stdinfo')
        ->fields('stdinfo',array('name','alt_email')) 
        ->condition('uid', $student_id, '=') 
        ->execute()->fetchAll();     
  return $studentname;  
}