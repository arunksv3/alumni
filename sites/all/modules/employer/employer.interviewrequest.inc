<?php

function employer_interview_state_list_ajax($form, $form_state) {
    // Return the dropdown list including the wrapper
    return $form['address']['wrapper'];
}

function employer_interviewrequest_submit($form, &$form_state) {
    global $user, $base_url;
    /* Decrypting the data using configration key */
    $decrypting_key = variable_get('IIUM_Encryption_Key', NULL);
    $student_id = (int) decrypt($form_state['values']['student_id'], $decrypting_key);
    $job_id = (int) decrypt($form_state['values']['job_id'], $decrypting_key);
    if (isset($form_state['values']['state']))
        $state = $form_state['values']['state'];
    else
        $state = 0;

    $from_input = explode('/', $form_state['values']['from']);
    $interview_date_from = $from_input[2] . '-' . $from_input[1] . '-' . $from_input[0];
    
    $from_to = explode('/', $form_state['values']['to']);
    $interview_date_to = $from_to[2] . '-' . $from_to[1] . '-' . $from_to[0];
        
    $employer_interviewrequest = db_insert('student_interview_request')
            ->fields(array(
                'employer_id' => $user->uid,
                'student_id' => trim($student_id),
                'jobid' => trim($job_id),
                'job_title' => trim($form_state['values']['subject']),
                'company_name' => trim($form_state['values']['company']),
                'email_id' => trim($form_state['values']['email_id']),
                'message' => trim($form_state['values']['message_all']),
                'salary_details' => trim($form_state['values']['salary_details']),
                'interview_date_from' => trim($interview_date_from),
                'interview_date_to' => trim($interview_date_to),
                'interview_time_from' => trim($form_state['values']['time_from']),
                'interview_time_to' => trim($form_state['values']['time_to']),
                'street' => trim($form_state['values']['street']),
                'town' => trim($form_state['values']['town']),
                'city' => trim($form_state['values']['city']),
                'country' => trim($form_state['values']['country']),
                'state' => trim($state),
                'zipcode' => trim($form_state['values']['zip_code']),
                'federal_title' => trim($form_state['values']['federal_title']),
                'p_title' => trim($form_state['values']['title']),
                'contactperson_fname' => trim($form_state['values']['fname']),
                'contactperson_lname' => trim($form_state['values']['lname']),
                'designation' => trim($form_state['values']['designation']),
                'company' => trim($form_state['values']['company']),
                'contact_call' => trim($form_state['values']['contact_call']),
                'email' => trim($form_state['values']['email']),
                'status' => 2,
                'created_date' => trim(date('Y-m-d H:i:s')),
            ))
            ->execute();

    //user status function
    $user_status_history = user_status_history($user->uid, $student_id, $job_id, 0, 'employer');
    //audit log function
    audit_log('sent interview request', 'User ID:'.$student_id); 
    
    $studentname = job_studentname_jobid($student_id);

    $path = $base_url . '/' . drupal_get_path('theme', 'bootstrap_business');
    $body = '<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>IIUM Email</title>
</head>

<body style="font-family:Arial, Helvetica, sans-serif; font-size:14px; line-height:30px; margin:0; padding:0;">
<table width="600" border="0" cellspacing="0" cellpadding="0" style="margin:0 auto; border: 1px solid #ccc; padding:0 0 15px 0;">
  <tr>
    <td style="background:url(' . $path . '/images/header-bg.jpg) repeat-x left top;border-bottom: 5px solid #9c2b3d; padding: 15px 10px; text-align: center;"><img src="' . $path . '/images/logo.png" width="228" height="83" /></td>
  </tr>
  <tr>
    <td style="padding: 0px 15px;"><h1 style="padding: 0px 0 5px; margin: 10px 0px 0; font-family: arial; font-weight: 600; color: #663398; font-size: 20px; border-bottom:1px solid #ccc;">Interview Request -<span style="font-size:15px; color:#000; font-weight:normal;"> This Request Expires in 24 hours of the delivery</span></h1></td>
  </tr>
  <tr>
    <td><table width="600" border="0" cellspacing="0" cellpadding="0" style="padding:10px 15px;">
   <tr>
    <td width="161"><b>Job ID:</b></td>
    <td width="439">' . $form_state['values']['jobid'] . '</td>
  </tr>  
  <tr>
    <td width="161"><b>Job Title:</b></td>
    <td width="439">' . $form_state['values']['subject'] . '</td>
  </tr>
  <tr>
    <td valign="top"><b>Message:</b></td>
    <td><p style="margin: 0px; line-height: 24px;">Dear ' . $studentname[0]->name . ',<br>' . $form_state['values']['message_all'] . '</p></td>
  </tr>
  <tr>
    <td><b>Salary Details:</b></td>
    <td>' . $form_state['values']['salary_details'] . '</td>
  </tr>
  <tr>
    <td valign="top"><b>We are conducting walk-in Interviews:</b></td>
    <td>' . str_replace("/", "-", $form_state['values']['from']) . '&nbsp;' . $form_state['values']['time_from'] . '&nbsp;&nbsp; To &nbsp;&nbsp;' . str_replace("/", "-", $form_state['values']['to']) . '&nbsp;' . $form_state['values']['time_to'] . '</td>
  </tr>
  <tr>
    <td colspan="2"><h2 style="color:#9c2b3d; padding: 0px 0px 5px; border-bottom: 1px solid #ccc; margin: 8px 0px 7px; font-weight: 600; font-size: 17px; line-height: 22px;">Interview Venue:</h2></td>
  </tr>
  <tr>
    <td><b>Street:</b></td>
    <td>' . $form_state['values']['street'] . '
</td>
  </tr>
  <tr>
    <td><b>City:</b></td>
    <td>' . $form_state['values']['city'] . '</td>
  </tr>
  <tr>
    <td><b>Country:</b></td>
    <td>' . get_country_name($form_state['values']['country']) . '</td>
  </tr>
  <tr>
    <td><b>State:</b></td>
    <td>' . get_state_name($state) . '</td>
  </tr>
  <tr>
    <td><b>Zip code:</b></td>
    <td>' . $form_state['values']['zip_code'] . '</td>
  </tr>
  <tr>
    <td colspan="2"><h2 style="color:#9c2b3d; padding: 0px 0px 5px; border-bottom: 1px solid #ccc; margin: 8px 0px 7px; font-weight: 600; font-size: 17px; line-height: 22px;">Contact Details:</h2></td>
  </tr>
  <tr>
    <td><b>Name:</b></td>
    <td>' . $form_state['values']['fname'] . ' ' . $form_state['values']['lname'] . '</td>
  </tr>
  <tr>
    <td><b>Designation:</b></td>
    <td>' . $form_state['values']['designation'] . '</td>
  </tr>
  <tr>
    <td><b>Company:</b></td>
    <td>' . $form_state['values']['company'] . '</td>
  </tr>
  <tr>
    <td><b>Call:</b></td>
    <td>' . $form_state['values']['contact_call'] . '</td>
  </tr>
  <tr>
    <td><b>Email:</b></td>
    <td>' . $form_state['values']['email'] . '</td>
  </tr>
</table>
</td>
  </tr>
</table>
</body>
</html>';


    $my_module = 'foo';
    $my_mail_token = 'bar';
    $from = $form_state['values']['email_id'];

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


    if ($system->mail($message)) {
        drupal_set_message('Your Interview request is sucessfully sent to the shortlisted applicant, this request will get expired in next 24 hours if applicant do not respond to it.', 'status');
        echo '<script type="text/javascript">  setTimeout(function(){window.close();  window.opener.location.reload();  }, 200); </script>';
        exit;
    } else {
        drupal_set_message('Mail error', 'error');
    }
}

/* View applicants function */

function employer_interviewrequest($form, &$form_state, $jobid, $applicantid) {
    global $user;
    if ($user->uid == 0)
        drupal_goto('user');

    /* Decrypting the data using configration key */
    $decrypting_key = variable_get('IIUM_Encryption_Key', NULL);

    $employer_details_query = db_select('employer_jobs_contact_person', 'c');
    $employer_details_query->innerjoin('employer_jobs', 'e', 'e.id = c.employer_jobid');
    $employer_details_query->fields('c', array());
    $employer_details_query->fields('e', array('job_title', 'jobid'));
    $employer_details_query->condition('c.employer_jobid', (int) decrypt($jobid, $decrypting_key), '=');
    $employer_details = $employer_details_query->execute()
            ->fetchAll();

    $comapany_name = db_select('employer_registration')
            ->fields('employer_registration', array('company_organization'))
            ->condition('uid', $user->uid, '=')
            ->execute()
            ->fetchAll();

    drupal_add_js(drupal_get_path('theme', 'bootstrap_business') . '/js/jquery.validate.js');
    drupal_add_js(drupal_get_path('theme', 'bootstrap_business') . '/js/jqueryformvalidation.js');
    drupal_add_js(drupal_get_path('module', 'employer') . '/js/interview_custom_validations.js');
    drupal_add_css(drupal_get_path('theme', 'bootstrap_business') . '/css/jquery-ui.css');
    drupal_add_js(drupal_get_path('theme', 'bootstrap_business') . '/js/jquery-ui.js');
    drupal_add_js('jQuery(document).ready(function($) { 
$("#from").datepicker({
 changeMonth: true,
 changeYear: true,
 showOn: "both",
 minDate: "0d",
 dateFormat:"dd/mm/yy",
 buttonImage: "' . file_create_url('public://calendar.gif') . '",
 buttonImageOnly: true,
 onSelect: function(selected) {
   $("#to").datepicker("option","minDate", selected)
  }
});
$("#to").datepicker({
 changeMonth: true,
 changeYear: true,
 showOn: "both",
 minDate: "0d",
 dateFormat:"dd/mm/yy",
 buttonImage: "' . file_create_url('public://calendar.gif') . '",
 buttonImageOnly: true,
 onSelect: function(selected) {
    $("#from").datepicker("option","maxDate", selected)
   }
});
});', 'inline');
    drupal_add_js("function printpage(){
            window.print();
         }
		 function closewindow(){
		    window.close(); 
		 }
		 ", 'inline');

    $form = array();
    $form['student_id'] = array(
        '#type' => 'hidden',
        '#value' => $applicantid,
    );
    $form['job_id'] = array(
        '#type' => 'hidden',
        '#value' => $jobid,
    );
    $form['jobid'] = array(
        '#type' => 'hidden',
        '#value' => (isset($employer_details[0]->jobid) ? $employer_details[0]->jobid : ''),
    );
    $form['send_interview'] = array(
        '#type' => 'item',
        '#markup' => '<h1>Send Interview Request</h1>',
    );
    $form['send_status'] = array(
        '#type' => 'item',
        '#markup' => '<div class="note">Note:This Request Expires in 24 hours of the delivery</div>',
    );
    $form['email_id'] = array(
        '#type' => 'textfield',
        '#title' => 'Your Email ID',
        '#required' => TRUE,
        '#default_value' => (isset($employer_details[0]->email) ? $employer_details[0]->email : ''),
    );
    $form['subject'] = array(
        '#type' => 'textfield',
        '#title' => 'Subject/Job Title',
        '#required' => TRUE,
        '#default_value' => (isset($employer_details[0]->job_title) ? $employer_details[0]->job_title : ''),
    );
    $form['message'] = array(
        '#type' => 'item',
        '#markup' => '<h2>Message</h2>',
    );
    $form['message_desc'] = array(
        '#type' => 'item',
        '#markup' => 'Please enter details of the job for which you are contacting the candidates.This information is shown upfront in the mail sent
         to jobseekers,so it is recommended that you fill these fields',
    );
    $form['message_all'] = array(
        '#type' => 'textarea',
        '#id' => 'message_all',
        '#title' => 'Message',
        '#required' => TRUE,
        '#attributes' => array('placeholder' => 'Congratulations!! Your profile got short listed and it is a request for interview for our job profile ..... '),
    );
    $form['salary_details'] = array(
        '#type' => 'textfield',
        '#id' => 'salary_details',
        '#title' => 'Salary Details',
    );
    $form['filter']['daterange'] = array(
        '#type' => 'fieldset',
        '#title' => t('We are conducting walk-in Interviews'),
    );
    $form['filter']['daterange']['from'] = array(
        '#type' => 'textfield',
        '#id' => 'from',
        '#title' => 'From',
        '#required' => TRUE,
        '#attributes' => array('readonly' => 'readonly'),
    );
    $form['filter']['daterange']['to'] = array(
        '#type' => 'textfield',
        '#id' => 'to',
        '#title' => 'To',
        '#required' => TRUE,
        '#attributes' => array('readonly' => 'readonly'),
    );
    $form['filter']['time'] = array(
        '#type' => 'fieldset',
        '#title' => t('Interview Timings between'),
    );
    $form['filter']['time']['time_from'] = array(
        '#type' => 'select',
        '#id' => 'from',
        '#title' => 'From',
        '#options' => datetime(),
    );
    $form['filter']['time']['time_to'] = array(
        '#type' => 'select',
        '#id' => 'to',
        '#title' => 'To',
        '#options' => datetime(),
    );
    $form['address'] = array(
        '#type' => 'fieldset',
    );
    $form['address']['street'] = array(
        '#title' => 'Street',
        '#type' => 'textfield',
        '#required' => TRUE,
        '#default_value' => (isset($street) ? $street : ''),
        '#maxlength' => 50
    );
    $form['address']['city'] = array(
        '#title' => 'City',
        '#type' => 'textfield',
        '#required' => TRUE,
        '#default_value' => (isset($city) ? $city : ''),
        '#maxlength' => 50
    );
    $form['address']['country'] = array(
        '#type' => 'select',
        '#title' => t('Country'),
        '#options' => array('' => 'Select') + countries_list(),
        '#required' => TRUE,
        '#ajax' => array(
            'event' => 'change',
            'callback' => 'employer_interview_state_list_ajax',
            'wrapper' => 'state-list-wrapper',
        ),
        '#default_value' => (isset($country) ? $country : ''),
    );

    // Wrapper for state dropdown list
    $form['address']['wrapper'] = array(
        '#prefix' => '<div id="state-list-wrapper">',
        '#suffix' => '</div>',
    );

    if ((isset($form_state['values']['country']) && $form_state['values']['country'] != "")) {
        if (!isset($form_state['values']['country'])) {
            $cid = $country;
        } else {
            $cid = $form_state['values']['country'];
        }
        $state_list = db_select('states_list')
                ->fields('states_list')
                ->condition('cid', $cid, '=')
                ->execute()
                ->fetchAll();
        $state_list_arr[''] = 'Select';
        foreach ($state_list as $value):
            $state_list_arr[$value->id] = $value->name;
        endforeach;
        if (count($state_list) > 0) {
            // Province dropdown list
            $form['address']['wrapper']['state'] = array(
                '#type' => 'select',
                '#title' => t('State'),
                '#options' => $state_list_arr,
                '#required' => TRUE,
                '#default_value' => (isset($state) ? $state : ''),
            );
        }
    }

    $form['address']['zip_code'] = array(
        '#title' => 'Zip code',
        '#type' => 'textfield',
        '#required' => TRUE,
        '#default_value' => (isset($zipcode) ? $zipcode : ''),
        '#maxlength' => 5
    );
    $form['contactperson_details'] = array(
        '#type' => 'item',
        '#markup' => '<h2>Contact Person Details</h2>',
    );
    $form['federal_title'] = array(
        '#title' => 'Federal/State title',
        '#type' => 'select',
        '#options' => array('' => 'Select') + comp_titles(),
        '#required' => TRUE,
        '#default_value' => (isset($employer_details[0]->federal_title) ? $employer_details[0]->federal_title : ''),
    );
    $form['title'] = array(
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
        '#default_value' => (isset($employer_details[0]->p_title) ? $employer_details[0]->p_title : ''),
        '#required' => TRUE, //make this field required
    );
    $form['fname'] = array(
        '#type' => 'textfield',
        '#id' => 'fname',
        '#title' => 'Firstname',
        '#maxlength' => 20,
        '#default_value' => (isset($employer_details[0]->firstname) ? $employer_details[0]->firstname : ''),
        '#required' => TRUE,
    );
    $form['lname'] = array(
        '#type' => 'textfield',
        '#id' => 'lname',
        '#title' => 'Lastname',
        '#default_value' => (isset($employer_details[0]->lastname) ? $employer_details[0]->lastname : ''),
        '#maxlength' => 20,
        '#required' => TRUE,
    );
    $form['designation'] = array(
        '#type' => 'textfield',
        '#id' => 'designation',
        '#title' => 'Designation',
        '#default_value' => (isset($employer_details[0]->designation) ? $employer_details[0]->designation : ''),
        '#maxlength' => 30,
        '#required' => TRUE,
    );
    $form['company'] = array(
        '#type' => 'textfield',
        '#id' => 'company',
        '#title' => 'Company',
        '#default_value' => (isset($comapany_name[0]->company_organization) ? $comapany_name[0]->company_organization : ''),
        '#maxlength' => 70,
        '#required' => TRUE,
    );
    $form['contact_call'] = array(
        '#type' => 'textfield',
        '#id' => 'contact_call',
        '#default_value' => (isset($employer_details[0]->telephone) ? $employer_details[0]->telephone : ''),
        '#title' => 'Call',
        '#maxlength' => 30,
    );
    $form['email'] = array(
        '#type' => 'textfield',
        '#id' => 'email',
        '#title' => 'Email',
        '#default_value' => (isset($employer_details[0]->email) ? $employer_details[0]->email : ''),
        '#maxlength' => 50,
    );
    $form['submit'] = array(
        '#type' => 'submit',
        '#id' => 'submit',
        '#value' => 'submit',
        '#submit' => array('employer_interviewrequest_submit'),
        '#prefix' => '<div class="interview-buttons">',
    );
    $form['button'] = array(
        '#type' => 'button',
        '#id' => 'button',
        '#value' => 'Close',
        '#attributes' => array("onclick" => "closewindow()"),
        '#suffix' => '</div>'
    );
    return $form;
}

function datetime() {
    return array(
        '' => t('Select Time'),
        '1 am' => t('1 am'),
        '2 am' => t('2 am'),
        '3 am' => t('3 am'),
        '4 am' => t('4 am'),
        '5 am' => t('5 am'),
        '6 am' => t('6 am'),
        '7 am' => t('7 am'),
        '8 am' => t('8 am'),
        '9 am' => t('9 am'),
        '10 am' => t('10 am'),
        '11 am' => t('11 am'),
        '12 pm' => t('12 pm'),
        '1 pm' => t('1 pm'),
        '2 pm' => t('2 pm'),
        '3 pm' => t('3 pm'),
        '4 pm' => t('4 pm'),
        '5 pm' => t('5 pm'),
        '6 pm' => t('6 pm'),
        '7 pm' => t('7 pm'),
        '8 pm' => t('8 pm'),
        '9 pm' => t('9 pm'),
        '10 pm' => t('10 pm'),
        '11 pm' => t('11 pm')
    );
}

/**
 * Get Student name in student job details table based on employer table
 */
function job_studentname_jobid($student_id) {
    $studentname = db_select('stdinfo')
                    ->fields('stdinfo', array('name', 'alt_email'))
                    ->condition('uid', $student_id, '=')
                    ->execute()->fetchAll();
    return $studentname;
}