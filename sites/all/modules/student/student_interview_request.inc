<?php 
function student_interview_request($form, &$form_state) { 
    drupal_add_js(drupal_get_path('module', 'student') .'/js/custom.js'); 
        
    global $user;
    global $base_url;
    $output = "";    
    $key = variable_get('IIUM_Encryption_Key', NULL);
    
    $output .= '<p> Interview requests from employer will expire in 24 hours of request sent, Please accept/reject on the request sent accrodingly </p>';
    
    if(isset($_GET['accept'])){
        $accept = $_GET['accept'];
        $jid = $_GET['jobid'];
        $accept_id = (int)decrypt($accept, $key);
        $jid = (int)decrypt($jid, $key);
        
        $e_update = db_update('student_interview_request') 
           ->fields(array(              
                   'status' => '0',                     
               ))
           ->condition('id', $accept_id, '=')
           ->execute();
        
        /* Maintain status histoty in user_status_history */
        if($e_update){
            user_status_history('0', $user->uid, $jid, '0', 'student');
            drupal_set_message('You have been successfully Accepted the Interview Request.');
            drupal_goto('student/interview_request');
        }
    }
     
    if(isset($_GET['reject'])){
        $reject = $_GET['reject'];
        $jid = $_GET['jobid'];
        $reject_id = (int)decrypt($reject, $key);
        $jid = (int)decrypt($jid, $key);
        
        $e_update = db_update('student_interview_request') 
           ->fields(array(              
                   'status' => '3',                     
               ))
           ->condition('id', $reject_id, '=')
           ->execute();
        
        /* Maintain status histoty in user_status_history */
        if($e_update){
            user_status_history('0', $user->uid, $jid, '3', 'student');
            drupal_set_message('You have been successfully Rejected the Interview Request.');
            drupal_goto('student/interview_request');
        }
     }
    
    
    /* Required information for resume request */
    $std_int_reqs = db_select('student_interview_request', 's');
    $std_int_reqs->innerjoin('student_job_details', 'j', 's.student_id = j.uid and  s.jobid = j.sjid');
    $std_int_reqs->innerjoin('employer_jobs', 'ej', 's.jobid = ej.id');
    $std_int_reqs->innerjoin('employer_registration', 'er', 's.employer_id = er.uid');
    $std_int_reqs->fields('s', array('id','job_title','company_name','created_date','status'));
    $std_int_reqs->fields('j', array('status'));
    $std_int_reqs->fields('ej', array('jobid'));
    $std_int_reqs->fields('er', array('company_logo','company_logo_name','company_organization'));
    $std_int_reqs->condition('s.student_id', $user->uid, '=');
    $std_int_reqs->condition('j.status', '3', '!=');    

    if(isset($_GET)){
        if(isset($_GET['view'])){
            $view = $_GET['view'];
            $status_view = (int)decrypt($view, $key); 
            if($status_view == "0"){
                $std_int_reqs->condition('s.status', '1', '!=');
            }else if($status_view == "1"){
                $std_int_reqs->condition('s.status', '1', '=');
            }            
        }
        if(isset($_GET['sort'])){
            $sort = $_GET['sort'];
            if($sort=='0'){ 
                $std_int_reqs->orderBy('s.job_title', 'ASC');
            }else if($sort=='1'){
                $std_int_reqs->orderBy('company_name', 'ASC');
            }else if($sort=='2') {
                $std_int_reqs->orderBy('created_date', 'ASC');
            }else if($sort=='3') {
                $std_int_reqs->orderBy('status', 'ASC');
            }
        }        
    }
    
    $std_int_reqs->orderBy('s.id', 'DESC');    
    $std_int_info = $std_int_reqs->execute()->fetchAll(); 
    
    //$std_int_info_count = count($std_int_info);
    //echo '<pre>'; print_r($std_int_info); exit; 
    
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
        $status = interview_status($value->status);
        $employer_student_status = employer_student_status();
        
        /* status links */
        if($value->status != '2' || $value->j_status == '4')
            $options .= '<span>Accept Interview Request</span>';            
        else
            $options .= l('Accept Interview Request', $base_url.'/student/interview_request?accept='.encrypt($value->id,$key).'&jobid='.encrypt($value->jobid,$key));
        if($value->status != '2' || $value->j_status == '4')
            $options .= '<span>Reject Interview Request</span>'; 
        else
            $options .= l('Reject Interview Request', $base_url.'/student/interview_request?reject='.encrypt($value->id,$key).'&jobid='.encrypt($value->jobid,$key),  array('attributes' => array('onclick' => "return confirm('Are you sure you want to reject the Interview Request for this job')")));
        $options .= l('View Interview Request', $base_url.'/student/viewrequest/interview/'.encrypt($value->id,$key), array('attributes' => array('class' => "popup")));
        
        $options .= '<span class="final-status">Employer Action: '.$employer_student_status[$value->j_status].'</span>';        
        $options .= '</div>';
        $reslut[] = array($value->jobid, $value->job_title, $value->company_organization, $image_tag, $request_sent_on, $status, $options);
    }
   
    $header = array(
        array('data' => t('Job Id')), 
        array('data' => t('Job Title')),
        array('data' => t('Company Name')),
        array('data' => t('Company Logo')),
        array('data' => t('Request sent on')),
        array('data' => t('Interview Request Status')),
        array('data' => t('Options')),
    );
    
    //if($std_int_info_count > 0){    
        $links = ''.l('Active request', $base_url.'/student/interview_request?view='.encrypt('0',$key)).'| 
                    '.l('Inactive request', $base_url.'/student/interview_request?view='.encrypt('1',$key)).'';
        
        
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
            '#options' => array('Job Title','Company Name', 'Request Date', 'Status'), 
            '#attributes' => array('onchange' => "document.location='".$base_url."/student/interview_request/?sort='+this.value;"),
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