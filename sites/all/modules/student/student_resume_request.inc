<?php 
function student_resume_request($form, &$form_state) {    
    drupal_add_js(drupal_get_path('module', 'student') .'/js/custom.js');
    
    global $user;
    global $base_url;
    $output = "";
    $options = "";
    $key = variable_get('IIUM_Encryption_Key', NULL);
    
    $output .= '<p> Resume Requests from employer for visibility access of your Resume will expire in 24 hours of request sent, 
                    Please Accept / Reject it accordingly. Make sure you provide the visiblity permission in case you accept the request </p>';
    
    if(isset($_GET['accept'])){
        $accept = $_GET['accept'];        
        $accept_id = (int)decrypt($accept, $key);        
        
        $e_update = db_update('student_resume_request') 
           ->fields(array(              
                   'status' => '0',                     
               ))
           ->condition('id', $accept_id, '=')
           ->execute();       
        
        /* Maintain status histoty in user_status_history */
        if($e_update){
            user_status_history('0', $user->uid, '0', '0', 'student');
            drupal_set_message('You have been successfully Accepted the Resume Request.');
            drupal_goto('student/resume_request');
        }
     }
     
     if(isset($_GET['reject'])){
        $reject = $_GET['reject'];        
        $reject_id = (int)decrypt($reject, $key);
        
        $e_update = db_update('student_resume_request') 
           ->fields(array(              
                   'status' => '3',                     
               ))
           ->condition('id', $reject_id, '=')
           ->execute();
        
        /* Maintain status histoty in user_status_history */
        if($e_update){
            user_status_history('0', $user->uid, '0','3', 'student');
            drupal_set_message('You have been successfully Rejected the Resume Request.');
            drupal_goto('student/resume_request');
        }
     }
    
    
    /* Required information for resume request */    
    $std_res_reqs = db_select('student_resume_request', 's');    
    $std_res_reqs->innerjoin('employer_registration', 'er', 's.employer_id = er.uid');
    $std_res_reqs->fields('s', array('id','student_id','created_date','status'));    
    $std_res_reqs->fields('er', array('company_logo','company_logo_name','company_organization'));
    $std_res_reqs->condition('s.student_id', $user->uid, '=');    

    if(isset($_GET)){
        if(isset($_GET['view'])){
            $view = $_GET['view'];
            $status_view = (int)decrypt($view, $key); 
            if($status_view == "0"){
                $std_res_reqs->condition('s.status', '1', '!=');
            }else if($status_view == "1"){
                $std_res_reqs->condition('s.status', '1', '=');
            }
        }
        if(isset($_GET['sort'])){
            $sort = $_GET['sort'];
            if($sort=='0'){
                $std_res_reqs->orderBy('er.company_organization', 'ASC');
            }else if($sort=='1') {
                $std_res_reqs->orderBy('s.created_date', 'ASC');
            }else if($sort=='2') {
                $std_res_reqs->orderBy('s.status', 'ASC');
            }
        }        
    }
    
    $std_res_reqs->orderBy('s.id', 'DESC');    
    $std_int_info = $std_res_reqs->execute()->fetchAll();
    
    //$std_int_info_count = count($std_int_info);
    
    $reslut = array();
    foreach ($std_int_info as $value) {
        $options = "<div class='options'>";             
        $variables = array(
            'path' => $value->company_logo, 
            'alt' => $value->company_logo_name,
            'title' => $value->company_logo_name,
            'width' => '150',
            'height' => '150',            
        );
        $image_tag = theme('image', $variables);
        $request_sent_on = dateformat($value->created_date);
        $status = resume_status($value->status);   
        $employer_student_status = employer_student_status();
        
        /* status links */
        if($value->status != '2')
            $options .= '<span>Accept Resume Request</span>';            
        else
            $options .= l('Accept Resume Request', $base_url.'/student/resume_request?accept='.encrypt($value->id,$key));
        if($value->status != '2')
            $options .= '<span>Reject Resume Request</span>'; 
        else
            $options .= l('Reject Resume Request', $base_url.'/student/resume_request?reject='.encrypt($value->id,$key),  array('attributes' => array('onclick' => "return confirm('Are you sure you want to reject the Resume Request for this job')")));
        $options .= l('View Resume Request', $base_url.'/student/viewrequest/resume/'.encrypt($value->id,$key), array('attributes' => array('class' => "popup")));
        
        $options .= '</div>';
        $reslut[] = array($value->company_organization, $image_tag, $request_sent_on, $status, $options);
    }
    
    $header = array(
        array('data' => t('Company Name')),
        array('data' => t('Company Logo')),
        array('data' => t('Request sent on')),
        array('data' => t('Resume Request Status')),
        array('data' => t('Options')),
    );
    
    //if($std_int_info_count > 0){    
        $links = ''.l('Active request', $base_url.'/student/resume_request?view='.encrypt('0',$key)).' | 
                        '.l('Inactive request', $base_url.'/student/resume_request?view='.encrypt('1',$key)).'';

        $form['cutomtext'] = array(
            '#type' => 'item',
            '#markup' => $output,       
        );

        $form['links'] = array(
            '#type' => 'item',
            '#markup' => $links,       
        );

        $form['sorting'] = array(             
            '#type' => 'select',
            '#title' => t('Sort by:'),
            '#options' => array('Company Name', 'Request Date', 'Status'), 
            '#attributes' => array('onchange' => "document.location='".$base_url."/student/resume_request/?sort='+this.value;"),
            '#default_value' => (isset($sort) ? $sort : ''), 
        );
    //}
    
    $form['table'] = array(
        '#theme' => 'table',
        '#header' => $header,
        '#rows' => $reslut,
        '#empty' => t('No Records Found!')
    );
    
    return $form;
    
}