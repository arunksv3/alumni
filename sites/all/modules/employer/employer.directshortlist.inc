<?php
/*Direct shortlist submit function*/
function employer_directshortlist_submit($form, &$form_state)
{
    global $user;
    /*Decrypting the data using configration key*/
    $decrypting_key=variable_get('IIUM_Encryption_Key', NULL);
    $student_id=(int)decrypt($form_state['values']['student_id'],$decrypting_key);
    $jobid = (int)decrypt($form_state['values']['jobid'],$decrypting_key);
    $employerid = $user->uid;
    
 $numberof_records = db_select('student_job_details')->fields(NULL, array('id'))->condition('sjid',$jobid,'=')->condition('uid',$student_id,'=')->countQuery()->execute()->fetchField();
    
 if($numberof_records>0)
 {
  drupal_set_message('This applicant already applied this job','error'); 
 } 
 else
 { 
    $employer = employerdetails($employerid);
    $jobinfo = jobinfo($jobid);
    $std_dtls = student_details($student_id, 'ASC');
    $std_exp_info = student_experience_result($student_id, 'ASC');
    $std_lang = student_languages($student_id, 'ASC');
    $std_qual = student_qualification($student_id, 'ASC');
    $std_skills = student_skills($student_id, 'ASC');
    $std_job_dtls = student_job_details($jobinfo['id'],$student_id, 'ASC');
    $std_job_exp_info = student_job_experience_info($jobinfo['id'],$student_id, 'ASC');
    $std_job_lan = student_job_languages($jobinfo['id'],$student_id, 'ASC');
    $std_job_qual = student_job_qualification($jobinfo['id'],$student_id, 'ASC');
    $std_job_skls = student_job_skills($jobinfo['id'],$student_id, 'ASC');
    
    
    //echo '<pre>'; print_r($std_exp_info); exit; 
    
    /*User status function*/
    $user_status_history=user_status_history($employerid,$student_id,$jobid,1,'employer');
    
      if(!empty($std_job_dtls))
        $db_std_job_status = 'db_update';        
    else
        $db_std_job_status = 'db_insert';    
        
    /* Insert into student_job_details table */
    $sjd = $db_std_job_status('student_job_details')           
                ->fields(array(  
                    'eid' => $employerid,
                    'uid' => $student_id,  
                    'sjid' => $jobid,
                    'job_title' => $jobinfo['job_title'],                
                    'company_name' => $employer['company_organization'],                                      
                    'company_logo' => $employer['company_logo'],
                    'make_your_pitch' => $form_state['input']['pitch'],
                    'firstname' => $std_dtls['firstname'],                
                    'lastname' => $std_dtls['lastname'],                                      
                    'dob' => $std_dtls['dob'],   
                    'country' => $std_dtls['country'],                
                    'state' => $std_dtls['state'],  
                    'nationality' => $std_dtls['nationality'],                    
                    'mobile' => $std_dtls['mobile'],    
                    'telephone' => $std_dtls['telephone'],
                    'email' => $std_dtls['email'],   
                    'photo' => $std_dtls['photo'],
                    'additional_info' => $std_dtls['additional_info'],
                    'experience_in_years' => $std_dtls['experience_in_years'],
                    'experience_in_months' => $std_dtls['experience_in_months'],
                    'searchable' => $std_dtls['searchable'],
                    'highest_qualification' => $std_dtls['highest_qualification'],
                    'highest_qualification_level' => $std_dtls['highest_qualification_level'],
                    'field_of_study' => $std_dtls['field_of_study'],
                    'intrested' => $std_dtls['intrested'],
                    'specialization_p' => $std_dtls['specialization_p'],
                    'specialization_sp' => $std_dtls['specialization_sp'],   
                    'specialization_c' => $std_dtls['specialization_c'],   
                    'work_locations' => $std_dtls['work_locations'],
                    'location_country' => $std_dtls['location_country'],
                    'location_state' => $std_dtls['location_state'],
                    'expected_salary' => $std_dtls['expected_salary'],
                    'status'=> 1,
                    'working_year' => $std_dtls['working_year'],
                    'working_month' => $std_dtls['working_month'],
                    'experience_level' => $std_dtls['experience_level'],
                    'resume' => $std_dtls['resume'],
                    'created_date' => date('Y-m-d H:i:s'),
               ));
    
    if(!empty($std_job_dtls))
        $sjd->condition('sjid', $jobid, '=');
    
    $sjd = $sjd->execute();  
    

    if(!empty($std_job_exp_info))
        $std_job_exp_status = 'db_update';        
    else
        $std_job_exp_status = 'db_insert'; 
    
    /* Insert into student_job_experience_info table */
    foreach ($std_exp_info as $value) {
        $sje = $std_job_exp_status('student_job_experience_info')           
           ->fields(array(
                    'sjid' => $jobid,
                    'uid' => $student_id,   
                    'experience_level' => $value->experience_level,                
                    'working_since' => $value->working_since,   
                    'company_name' => $value->company_name,                
                    'country' => $value->country,   
                    'state' => $value->state,                
                    'industry' => $value->industry,  
                    'industry_other' => $value->industry_other,
                    'date_joined' => $value->date_joined,   
                    'date_left' => $value->date_left,
                    'present' => $value->present,
                    'job_title' => $value->job_title,  
                    'position_level' => $value->position_level,
                    'specialization_p' => $value->specialization_p,
                    'specialization_sp' => $value->specialization_sp,   
                    'specialization_c' => $value->specialization_c,
                    'salary' => $value->salary,
                    'description' => $value->description,
                    'created_date' => date('Y-m-d H:i:s'),
               ));
    
        if(!empty($std_job_exp_info))
            $sje->condition('sjid', $jobid, '=');

        $sje = $sje->execute(); 
    }
    
    
    if(!empty($std_job_lan))
        $std_job_lan_status = 'db_update';        
    else
        $std_job_lan_status = 'db_insert';  
    
    /* Insert into student_job_languages table */
    foreach ($std_lang as $value) {
        $sjl = $std_job_lan_status('student_job_languages')           
               ->fields(array(
                        'sjid' => $jobid,
                        'uid' => $student_id,   
                        'primary_language' => $value->primary_language,
                        'language' => $value->language,                
                        'spoken' => $value->spoken,   
                        'written' => $value->written,                
                        'certificate' => $value->certificate,  
                        'created_date' => date('Y-m-d H:i:s'),
                   ));    

        if(!empty($std_job_lan))
            $sjl->condition('sjid', $jobid, '=');

        $sjl = $sjl->execute();    
    }

    
    if(!empty($std_job_qual))
        $std_job_qual_status = 'db_update';        
    else
        $std_job_qual_status = 'db_insert';  
 
    /* Insert into student_job_qualification table */
    foreach ($std_qual as $value) {
        $sjq = $std_job_qual_status('student_job_qualification')           
               ->fields(array(
                        'sjid' => $jobid,  
                        'uid' => $student_id,    
                        'qualification' => $value->qualification,   
                        'qualification_level' => $value->qualification_level,   
                        'field_of_study' => $value->field_of_study,   
                        'university' => $value->university,                                        
                        'location_country' => $value->location_country,   
                        'location_state' => $value->location_state,   
                        'graduation_date' => $value->graduation_date,   
                        'additional_info' => $value->additional_info,
                        'highest' => $value->highest,   
                        'major' => $value->major,   
                        'grade' => $value->grade,   
                        'created_date' => date('Y-m-d H:i:s'),
                   ));

        if(!empty($std_job_qual))
            $sjq->condition('sjid', $jobid, '=');

        $sjq = $sjq->execute();
    }

       
    if(!empty($std_job_skls))
        $std_job_skls_status = 'db_update';        
    else
        $std_job_skls_status = 'db_insert';  
    /* Insert into student_job_skills table */
    foreach ($std_skills as $value) {
        $sjskl = $std_job_skls_status('student_job_skills')           
               ->fields(array(
                        'sjid' => $jobid, 
                        'uid' => $student_id,   
                        'skills' => $value->skills,                
                        'proficiency' => $value->proficiency, 
                        'created_date' => date('Y-m-d H:i:s'),
                   ));

        if(!empty($std_job_skls))
            $sjskl->condition('sjid', $jobid, '=');

        $sjskl = $sjskl->execute();
    }
    
    audit_log('shortlisted', 'user id: '.$student_id."<br> job id: ".$jobid);
    
    drupal_set_message('This candidate is Shortlisted for '.$jobinfo['job_title'].', you can send him the interview request from Shortlist&Interview requests tab'); 
 }
    echo '<script type="text/javascript">  setTimeout(function(){window.close();  window.opener.location.reload();  }, 200); </script>';
    exit; 

}
/*Direct shortlist function*/
function employer_directshortlist($form, &$form_state,$student_id)
{   

     global $user;
     if($user->uid==0)
      drupal_goto('user');
         
    drupal_add_js("function printpage(){
            window.print();
         }
		 function closewindow(){
		    window.close(); 
		 }
		 ",'inline');
     
    $form = array();
    $form['student_id'] = array(
    '#type' => 'hidden', 
    '#value' => $student_id,
    );
    $form['send_interview'] = array(
     '#type' => 'item',
     '#markup' => '<h2>Shortlist this candidate</h2>',  
    );
    $form['send_status'] = array(
     '#type' => 'item',
     '#markup' => 'Shortlist the candidate for your job opening',  
    );
    $form['jobid'] = array(
    '#type' => 'select', 
    '#title' => 'Select Job',
    '#required' => TRUE,
    '#options' => all_jobtitle_jobid_function($user->uid),
    );
   $form['submit'] = array(
    '#type' => 'submit', 
    '#id' => 'submit', 
    '#value' => 'submit',
    '#submit' => array('employer_directshortlist_submit'),
   );
  $form['button'] = array(
    '#type' => 'button', 
    '#id' => 'button', 
    '#value' => 'Close',
    '#attributes' => array("onclick" => "closewindow()"),
  );
    return $form;
    
}