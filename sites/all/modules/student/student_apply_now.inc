<?php
function student_apply_now($form, &$form_state, $jid, $uid) {
    global $user;
    $eid = $uid;
    $jobid = $jid;
    $disbaled = "";    
    $key = variable_get('IIUM_Encryption_Key', NULL);
    $jid = (int)decrypt($jid,$key);
    $uid = (int)decrypt($uid,$key);
    
    $jobinfo = jobinfo($jid);
    $employer = employer_name($uid);       
    $std_dtls = student_details($user->uid, 'ASC');  
    $positionArr = positionoffered();
    $location = get_country_name($jobinfo['job_country']). ($jobinfo['job_state']!="0" ? ', '.get_state_name($jobinfo['job_state']): '');    
    
    $std_job_dtls = db_select('student_job_details')
        ->fields('student_job_details')
        ->condition('sjid', $jid, '=')
        ->condition('uid', $user->uid, '=')
        ->orderBy('id', 'DESC')
        ->execute()
        ->fetchAll();   
    
    if(!empty($std_job_dtls) && !isset($_GET['msg']))
        drupal_set_message("You have already applied to this job! If you have updated your resume and wish to re-submit it to the employer, please continue.", 'error'); 
    
    if(empty($std_dtls)){
        $disbaled = "disabled";
        drupal_set_message("You didn't create your resume. Please create your resume to apply job", 'error'); 
    }
    
    $info_header = '<div>
                <h1>'.$jobinfo['job_title'].' - '.$employer['company_organization'].'</h1>
            </div>
            <ul>
                <li><label>Location:</label><span> '.$location.'</span></li>
                '.($jobinfo['year_of_experience']!=""? '<li><label>Experience:</label><span> '.$jobinfo['year_of_experience'] .' Year(s) ('.$positionArr[$jobinfo['position_offered']].')</span></li>': '').'                
                '.($jobinfo['salary']!="0"? '<li><label>Salary (MYR): </label><span>'.$jobinfo['salary'] .'</span></li>': '').'
            </ul>';
    
    $form['headertext'] = array(
        '#type' => 'item',
        '#markup' => $info_header,
    ); 
    
    $form['pitch'] = array(
        '#type' => 'textarea',
        '#title' => '<h2>Stand out from other applicants. Make your Pitch! (Recommended)</h2>',
        '#attributes' => array('placeholder' => 'Why do you think you can do well in this job? Tips: Be specific on how you can contribute. Avoid a generic pitch, e.g., I am hardworking.')
    );
    
    $form['eid'] = array(
        '#type' => 'hidden', 
        '#value' => $eid,
    );
    
    $form['jobid'] = array(
        '#type' => 'hidden', 
        '#value' => $jobid,
    );
    
    $info_sidebar = '<ul>
                        <li><label>MyIIUM Resume</label><span> '.l('View', $base_url.'student/resumepreview').' / '.l('Edit', $base_url.'student/editexperience').' </span></li>
                        <li><label>Expected Salary (MYR): </label><span> '.$std_dtls['expected_salary'].'</span></li>
                        <li><label>Email: 	</label><span> '.$std_dtls['email'].'</span></li>
                        <li><label>Mobile No.: </label><span> '.$std_dtls['mobile'].'</span></li>                        
                    </ul>';
    
    $form['sidebartext'] = array(
        '#type' => 'item',
        '#markup' => $info_sidebar,
    ); 
    
    $form['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Submit Application'), 
        '#submit' => array('student_apply_now_submit'),
        '#attributes' => array($disbaled => $disbaled ),
    );
    
    return $form;
}

function student_apply_now_submit($form, &$form_state){
    global $user;
    $eid = $form_state['input']['eid'];
    $key = variable_get('IIUM_Encryption_Key', NULL);
    $uid = (int)decrypt($eid,$key);
    $jobid = (int)decrypt($form_state['input']['jobid'],$key);
    $employer = employerdetails($uid);
    $jobinfo = jobinfo($jobid);
    $std_dtls = student_details($user->uid, 'ASC');
    $std_exp_info = student_experience_result($user->uid, 'ASC');
    $std_lang = student_languages($user->uid, 'ASC');
    $std_qual = student_qualification($user->uid, 'ASC');
    $std_skills = student_skills($user->uid, 'ASC');
    $std_job_dtls = student_job_details($jobinfo['id'],$user->uid, 'ASC');
    $std_job_exp_info = student_job_experience_info($jobinfo['id'],$user->uid, 'ASC');
    $std_job_lan = student_job_languages($jobinfo['id'],$user->uid, 'ASC');
    $std_job_qual = student_job_qualification($jobinfo['id'],$user->uid, 'ASC');
    $std_job_skls = student_job_skills($jobinfo['id'],$user->uid, 'ASC');
    
    /* Maintain status histoty in user_status_history */
    user_status_history($uid, $user->uid, $jobid, '0', 'student');
    
    if(!empty($std_job_dtls))
        $db_std_job_status = 'db_update';        
    else
        $db_std_job_status = 'db_insert';    
        
    /* Insert into student_job_details table */
    $sjd = $db_std_job_status('student_job_details')           
                ->fields(array(  
                    'eid' => $uid,
                    'uid' => $user->uid,  
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
                    'working_year' => $std_dtls['working_year'],
                    'working_month' => $std_dtls['working_month'],
                    'experience_level' => $std_dtls['experience_level'],
                    'resume' => $std_dtls['resume'],
                    'created_date' => date('Y-m-d H:i:s'),
               ));
    
    if(!empty($std_job_dtls))
        $sjd->condition('sjid', $jobid, '=');
    else
        audit_log('Student applied job ', $jobinfo['jobid']); 
    
    $sjd = $sjd->execute();     
    
    /* Delete from student_job_experience_info table */   
    $exp_delete = db_delete('student_job_experience_info')                          
                      ->condition('sjid', $jobid, '=')
                      ->condition('uid', $user->uid, '=')
                      ->execute();
    
    /* Insert into student_job_experience_info table */
    foreach ($std_exp_info as $value) { 
        $sje = db_insert('student_job_experience_info')           
           ->fields(array(
                    'sjid' => $jobid,
                    'uid' => $user->uid,   
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
        
        $sje = $sje->execute(); 
    }     
    
    /* Delete from student_job_languages table */   
    $lan_delete = db_delete('student_job_languages')                          
                ->condition('sjid', $jobid, '=')
                ->condition('uid', $user->uid, '=')
                ->execute();
    
    /* Insert into student_job_languages table */    
    foreach ($std_lang as $value) { 
        $sjl = db_insert('student_job_languages')           
               ->fields(array(
                        'sjid' => $jobid,
                        'uid' => $user->uid,   
                        'primary_language' => $value->primary_language,
                        'language' => $value->language,                
                        'spoken' => $value->spoken,   
                        'written' => $value->written,                
                        'certificate' => $value->certificate,  
                        'created_date' => date('Y-m-d H:i:s'),
                   ));   
        
        $sjl = $sjl->execute();  
    }
 
    /* Delete from student_job_qualification table */   
    $qual_delete = db_delete('student_job_qualification')                          
                      ->condition('sjid', $jobid, '=')
                      ->condition('uid', $user->uid, '=')
                      ->execute(); 
    
    /* Insert into student_job_qualification table */    
    foreach ($std_qual as $value) {
        $sjq = db_insert('student_job_qualification')           
               ->fields(array(
                        'sjid' => $jobid,  
                        'uid' => $user->uid,    
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

        $sjq = $sjq->execute();
    }
    
    /* Delete from student_job_skills table */ 
    $skill_delete = db_delete('student_job_skills')                          
                      ->condition('sjid', $jobid, '=')
                      ->condition('uid', $user->uid, '=')
                      ->execute(); 
    
    /* Insert into student_job_skills table */   
    foreach ($std_skills as $value) {   
        $sjskl = db_insert('student_job_skills')           
               ->fields(array(
                        'sjid' => $jobid, 
                        'uid' => $user->uid,   
                        'skills' => $value->skills,                
                        'proficiency' => $value->proficiency, 
                        'created_date' => date('Y-m-d H:i:s'),
                   ));
        
        $sjskl = $sjskl->execute();
    }
    
    drupal_get_messages();
    drupal_set_message('Your application has been sent successfully to '.$employer['company_organization'].' 
            The employer will contact you if you are short-listed. Thank you and good luck in your application!');  
    drupal_goto('student/application');
}