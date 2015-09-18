<?php
function student_online_application($form, &$form_state) {
    drupal_add_js(drupal_get_path('module', 'student') .'/js/custom.js'); 
    
    $output  = "";
    global $user;
    global $base_url;  
    $key = variable_get('IIUM_Encryption_Key', NULL);
    
    // withdraw the record based on sjid  
    if(isset($_GET) && isset($_GET['withdraw'])){
        $q = (int)decrypt($_GET['withdraw'],$key);        
        $sjd_update = db_update('student_job_details') 
            ->fields(array(              
                    'status' => '3',    
                    'withdraw_date' => date('Y-m-d H:i:s'),
                ))
            ->condition('sjid', $q, '=')
            ->execute();
        
        /* Maintain status histoty in user_status_history */
        if($sjd_update){
            user_status_history('0', $user->uid, $q, '3', 'student');                
            drupal_set_message('You have been successfully withdraw from this job!'); 
            drupal_goto('student/application');
        }
    }
   
    /* job applicants query */
    $current_date = date('Y-m-d H:i:s');
    $date_sixmonths = date('Y-m-d H:i:s', strtotime("$current_date -6 months"));
    $application_query  = db_select('student_job_details', 'sjd');
    $application_query->innerJoin('employer_jobs', 'ej', 'sjd.sjid = ej.id');
    $application_query->fields('sjd', array('sjid','company_logo', 'modified_date', 'company_name', 'job_title', 'status', 'withdraw_date'));
    $application_query->fields('ej', array('id','jobid'));
    $application_query->condition('sjd.uid', $user->uid, '=');
    $application_query->condition('sjd.created_date', $date_sixmonths, '>=');
    
    if(isset($_GET)){
        if(isset($_GET['view'])){
            $view = (int)decrypt($_GET['view'],$key);
            if($view==1){
                $db_or = db_or();            
                $db_or->condition('sjd.status', 1, '=');
                $db_or->condition('sjd.status', 0, '=');
                $db_or->condition('sjd.status', 4, '=');
                $application_query->condition($db_or);
            }
        }
        if(isset($_GET['sort'])){
            $sort = $_GET['sort'];
            if($sort=='0'){ 
                $application_query->orderBy('sjd.modified_date', 'ASC');
            }else if($sort=='1'){
                $application_query->orderBy('sjd.job_title', 'ASC');
            }else if($sort=='2') {
                $application_query->orderBy('sjd.company_name', 'ASC');
            }
        }
    }
    
    if(!isset($_GET['sort']))
        $application_query->orderBy('sjd.id', 'DESC');    
    
    $job_applicants = $application_query->execute()->fetchAll();
    
    $applicants_count = count($job_applicants); 
    
    $output .= '<div class="main-header">'.($applicants_count=="0"?'No ':$applicants_count).' online applications in the last 6 months'.'</div>
                    <div class="status-link">'.l('All applications', $base_url.'/student/application?view='.encrypt('0',$key)).' |
                    '.l('Active applications', $base_url.'/student/application?view='.encrypt('1',$key)).'</div>';
    
    if($applicants_count > 0){
        // pagination script starts    
        $per_page = 10;
        // Initialize the pager
        $current_page = pager_default_initialize($applicants_count, $per_page);
        // Split your list into page sized chunks
        $chunks = array_chunk($job_applicants, $per_page, TRUE);      
    
        foreach ($chunks[$current_page] as $applicants) {
            $sjid = encrypt($applicants->sjid,$key);        
            $variables = array(
                'path' => $applicants->company_logo,             
                'width' => '150',
                'height' => '150',            
            );
            $image_tag = theme('image', $variables); 
            $dateArr = explode(' ',$applicants->modified_date);        
            $applied_date = dateformat($dateArr['0']);
            $output .= '<div class="application-detail">
                            <div class="app-job-companyname">'.$applicants->company_name.'</div>
                            <div class="app-jobid">'.$applicants->jobid.'</div>
                            <div class="app-detail-left">
                                <div class="app-detail-img">'.$image_tag.'</div>
                            </div>
                            <div class="app-detail-right">
                                <div class="app-jobtitle">'.$applicants->job_title.'</div>
                            </div>
                            <div class="Shortlisted-sec">
                                <div class="applicants">'.($applicants->status=="3" ? get_status($applicants->status).' on '.dateformat($applicants->withdraw_date) : get_status($applicants->status)).'</div>
                                <div class="verticalLine">&nbsp;</div>
                                <div class="applied">Applied '.$applied_date.'</div>
                                <div class="verticalLine">&nbsp;</div>
                                <div class="show-hide"><span>Show</span><a class="show" target="'.$applicants->id.'"><i class="icon-down-arrow"></i></a></div>
                                <div id="option'.$applicants->id.'" class="preview_options">
                                '.l('View Submitted Resume', 'student/viewresume/sjid/'.$sjid, array('attributes' => array('class' => 'popup', 'title'=>'View Submitted Resume'))).'
                                '.($applicants->status=="0" ? l('Withdraw Resume', $base_url.'/student/application?withdraw='.$sjid, array('attributes' => array('onclick' => 'return withdraw();'))) : '').'                        
                                </div>
                            </div>
                        </div>';
        }
    
        // Show the pager
        $output .= theme('pager', array('quantity', $applicants_count)); 
    }
    
    $form['sorting'] = array(             
        '#type' => 'select',
        '#title' => t('Sort'),
        '#options' => array('Last Applied Date','Job Title','Company Name'), 
        '#attributes' => array('onchange' => "document.location='".$base_url."/student/application/?sort='+this.value;"),
        '#default_value' => (isset($sort) ? $sort : ''), 
    );
    
    $form['cutomtext'] = array(
        '#type' => 'item',
        '#markup' => $output,       
    );
    
    return $form;
}

/* Job applicants status */
function get_status($status){
    if($status == '0'){
        $status = 'Received';
    }else if($status == '1'){
        $status = 'Shortlisted';
    }else if($status == '2'){
        $status = 'This postion has been filled';
    }else if($status == '3'){
        $status = 'Withdrawn';
    }else if($status == '4'){
        $status = 'Selected';
    }
    return $status;
}