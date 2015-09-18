<?php

function student_preview_resume_form() {
    global $user;
    global $base_url;
    resume_preview_validation();
    $std_job_qual = student_qualification($user->uid, 'DESC');

    $states_list = alumni_std_states_list();
    $stdinfo = get_stdinfo($user->uid);
    $StdProgram = get_stdprogram($user->uid);
    $ProgramCode = $StdProgram['program'];
    $ProgramCodePrev = $StdProgram['prev_program'];

    $student_details = db_select('student_details')
            ->fields('student_details')
            ->condition('uid', $user->uid, '=')
            ->execute();
    $detcount = $student_details->rowCount();


    $student_qualification = db_select('student_qualification')
            ->fields('student_qualification')
            ->condition('uid', $user->uid, '=')
            ->orderBy('id', 'desc')
            ->execute();
    $qcount = $student_qualification->rowCount();


    $student_experience = db_select('student_experience')
            ->fields('student_experience')
            ->condition('uid', $user->uid, '=')
            ->execute();

    $student_details1 = db_select('student_details')
            ->fields('student_details')
            ->condition('uid', $user->uid, '=')
            ->execute();
    while ($row = $student_details1->fetchAssoc($student_details1)) {
        $experience_level = $row['experience_level'];
        $year = $row['working_year'];
        $month = $row['working_month'];
    }
    $position = '';
    $experience = '';
    if ($experience_level == 0) {
        $student_experience1 = db_select('student_experience')
                ->fields('student_experience')
                ->condition('uid', $user->uid, '=')
                ->condition('present', 1, '=')
                ->execute();
        $ecount = $student_experience1->rowCount();

        //if ($ecount > 0) {
        if ($year > 0)
            $years = date_diff(date_create(date('Y-m-d')), date_create($year . '-' . $month . '-01'))->y;
        else
            $years = 0;
        if ($month > 0)
            $months = date_diff(date_create(date('Y-m-d')), date_create($year . '-' . $month . '-01'))->m;
        else
            $months = 0;
        $experience = $years . ' Years ' . $months . ' Months';
        while ($row = $student_experience1->fetchAssoc($student_experience1)) {
            $position = $row['job_title'] . ' at ' . $row['company_name'];
        }
        //}
    } else if ($experience_level == 1) {
        $position = 'Fresh graduate seeking my first job';
    } else {
        $position = 'Student seeking internship or part-time jobs';
    }
//echo "exp=".$experience;
    $skills = db_select('student_skills')
            ->fields('student_skills')
            ->condition('uid', $user->uid, '=')
            ->execute();

    $skillsCount = $skills->rowCount();
    $skillset = array();
    if ($skillsCount > 0) {
        $proficiencyArr = proficiency();
        while ($row = $skills->fetchAssoc($skills)) {
            $skill = $row['skills'];
            $proficiency = $row['proficiency'];
            $skillset[$proficiencyArr[$row['proficiency']]][] = $row['skills'];
        }
    }
    // echo "<pre>";print_r($skillset);


    $languages = db_select('student_languages')
            ->fields('student_languages')
            ->condition('uid', $user->uid, '=')
            ->execute();
    $languagesCount = $languages->rowCount();
    $additionalInfo = db_select('student_additional_info')
            ->fields('student_additional_info')
            ->condition('uid', $user->uid, '=')
            ->execute();
    $additionalInfoCount = $additionalInfo->rowCount();

    /* $ldapinfo = db_select('ldap_user_info')
      ->fields('ldap_user_info')
      ->condition('uid', $user->uid, '=')
      ->execute();
      while ($row = $ldapinfo->fetchAssoc($ldapinfo)) {
      $givenname = $row['givenname'];
      } */
    $row = stdinfo($user->uid);
    $givenname = $row['name'];
    $markup = '
<div class="download-resume prev-resume"><a href="' . $base_url . '/student/downloadresume/' . encrypt($user->uid, variable_get('IIUM_Encryption_Key', NULL)) . '/' . encrypt(01, variable_get('IIUM_Encryption_Key', NULL)) . '">
    <i id="preview-icon-download" class="icon-download tool-icon"></i>
    Download Resume
    </a></div>
<div class="preview_resume">
<div class="resume-preview-container">

<div class="resume-formSection">';
    if ($detcount > 0) {
        while ($row = $student_details->fetchAssoc($student_details)) {
            $firstname = $row['firstname'];
            $lastname = $row['lastname'];
            $email = $row['email'];
            $age = date_diff(date_create($stdinfo['birth_date']), date_create('today'))->y;
            //$date = explode('-', $row['dob']);
            //$dob = $date[1] . '/' . $date[2] . '/' . $date[0];
            $country = get_country_name($row['country']);
            $state = get_state_name($row['state']);
            $mobile = $row['mobile'];
            $telephone = $row['telephone'];
            $additional_info = $row['additional_info'];
            $photo = $row['photo'];
            $searchable = $row['searchable'];
            $experience_in_years = $row['experience_in_years'];
            $experience_in_months = $row['experience_in_months'];
            $expected_salary = $row['expected_salary'];
            $modified_date = date('M d Y', strtotime($row['modified_date']));
            $markup.= '<div class="pageRow">
        <div class="resume-row-update">
        <div class="resume-update">Last Updated : ' . $modified_date . '</div>
        </div>
        
        <div class="resume-header">
            <div class="resume-header-left">&nbsp;</div>
                <div class="resume-header-top">
                    <h1>' . $givenname . ' </h1>
                    <h2>' . $position . '</h2>
                </div>
            </div>
            <div class="resume-header-inner">
                <div class="resume-section-top-left">
                    <div class="resume-photo">' . ($photo != "" ? '<img src = "' . file_create_url($photo) . '" >' : '<img src="' . file_create_url('public://uploads/profile_pictures/profile_pic.jpg') . '"/>') . '
                        <div class="exp-salary"><h1>Expected Salary :</h1> <h2>MYR ' . $expected_salary . '</h2>
                    </div>
                </div>
                
                
                
            </div>  
        
        <div class="resume-section-top">               
        <div class="resume-section-top-inner">
            <ul>';
            if ($experience != '') {
                $markup.='<li><label>Experience:</label>
                <span>  ' . $experience . ' </span> 
                </li> ';
            }
            $markup.='<li><label>Education: </label><span>' . $std_job_qual['0']->university . ' (' . date('Y', strtotime($std_job_qual['0']->graduation_date)) . ') 
                   ' . get_qualiafication_level_name($std_job_qual['0']->qualification_level) . ' 
                   ' . $std_job_qual['0']->location . '</span></li>';
            //' . ($std_job_qual['0']->field_of_study!="" ? $feildArr[$std_job_qual['0']->field_of_study] : '') . '</span></li>';
            $markup.=' <li><label>Nationality:</label> <span>' . $states_list[$stdinfo['ctzship']] . '</span></li>
                   <li><label>Student’s Matric number:</label> <span>' . $stdinfo['matric_no'] . '</span></li>
              
                    </ul>
                    </div>
                </div>
                
                
                <div class="resume-section-top-right">                     
                    <div class="pageRow">
                        <div class="colLeft">
                        <i class="icon-phone icon icon-align"></i>                           
                        </div>
                        <div class="colMiddle resume-summary-heading"> ' . $mobile . '</div>
                    </div>
                    <div class="pageRow">
                    <div class="colLeft">
                        <i class="icon-envelope icon"></i>                        
                        </div>
                        <div class="colMiddle resume-summary-heading">' . $email . '</div>
                     </div>
                    <div class="pageRow">
                    <div class="colLeft">
                        <i class="icon-user icon"></i>                        
                    </div>
                    <div class="colMiddle resume-summary-heading"> ' . $age . ' Years</div>
                      </div>
                    <div class="pageRow">
                    <div class="colLeft">
                        <i class="icon-marker-dot icon"></i>
                        
                    </div>
                    <div class="colMiddle resume-summary-heading"> ' . $country . ' ' . $state . '</div>
                       </div>
                </div> 
                
                </div>
                </div>';
        }
    }

    $markup.='<div class="pageRow">
                <div class="resume-detail"> 
                <div class="resume-detail-title"> 
                <div class="re-tit-left"><i class="icon-additional"></i></div>
                    <div class="re-tit-right">Experience</div>
                    </div>';

    $ecount = $student_experience->rowCount();
    $industryArr = nature_bussiness_arr();
    if ($ecount > 0) {
        while ($row = $student_experience->fetchAssoc($student_experience)) {
            $company_name = $row['company_name'];
            $date1 = explode('-', $row['date_joined']);
            $date_joined = $date1[1] . '/' . $date1[2] . '/' . $date1[0];
            $date2 = explode('-', $row['date_left']);
            if (sizeof($date2) > 1) {
                $date_left = $date2[1] . '/' . $date2[2] . '/' . $date2[0];
            }
            $location_country = $row['country'];
            $location_state = $row['state'];
            $job_title = $row['job_title'];
            $position = get_position_name($row['position_level']);
            $industry = $row['industry'];
            $specialization_p = $row['specialization_p'];
            $specialization_sp = $row['specialization_sp'];
            $specialization_c = $row['specialization_c'];
            $description = $row['description'];
            $salary = $row['salary'];
            $present = $row['present'];
            if ($present == 1)
                $duration = date_diff(date_create($date_joined), date_create(date('Y-m-d')))->y;
            else
                $duration = date_diff(date_create($date_joined), date_create($date_left))->y;
            $markup.= '  
                    <div class="resume-detail-summary">
                    <div class="resume-detail-title">
                    <div class="resume-detail-item-left">';
            if ($present == 1)
                $markup.='<div>' . date('M Y', strtotime($date_joined)) . ' - Present </div>';
            else
                $markup.='<div>' . date('M Y', strtotime($date_joined)) . ' - ' . date('M Y', strtotime($date_left)) . ' </div>';

            $markup.='<div>(' . $duration . ' Years)</div>
                    </div>
                    <div class="resume-detail-item-middle">
                     <h1>' . $job_title . '</h1>
                    <h2>' . $company_name . ' | ' . get_country_name($location_country) . ' ' . get_state_name($location_state) . '</h2>
                   
                    
                    <div><label>Monthly Salary : </label><span>' . $salary . ' MYR</span></div>
                    <div><span>' . $description . '</span></div> 
                 </div></div>
               </div>
                ';
        }
    }
//     <div> <span>'.getSpecializationSubparent($specialization_sp).'</span></div>
    $markup.= ' 
             
                 </div></div>
        <div class="pageRow">
            <div class="resume-detail">
                <div class="resume-detail-title">
                <div class="re-tit-left"><i class="icon-education"></i></div>
                <div class="re-tit-right">Education</div></div>	';
    if ($qcount > 0) {
        $gradeArr = grade_list();
        while ($row = $student_qualification->fetchAssoc($student_qualification)) {
            $qualification = $row['qualification'];
            $qualification_level = $row['qualification_level'];
            $field_of_study = $row['field_of_study'];
            $university = $row['university'];
            $location = $row['location'];
            $location_country = $row['location_country'];
            $location_state = $row['location_state'];
            $graduation_date = $row['graduation_date'];
            $major = $row['major'];
            $grade = $gradeArr[$row['grade']];
            $additional_info = $row['additional_info'];
            
            $markup.=
                    ' <div class="resume-detail-summary">
                <div class="resume-detail-title">
                <div class="resume-detail-item-left">
                <div>' . date('Y', strtotime($graduation_date)) . '</div>
                </div>
                    <div class="resume-detail-item-middle">
                        <div> <h1>' . $university . '</h1></div>
                        <div> <h2>' . get_qualiafication_name($qualification) . ' - ' . get_qualiafication_level_name($qualification_level) . '</h2></div>
                        <div><h2> in ' . fields_of_study_name($field_of_study) . ' | ' . get_country_name($location_country) . ' ' . get_state_name($location_state) . '</h2></div>';
            if ($major != '') {
                $markup.= '<div> <label>Major: &nbsp; </label><span>' . $major . '</span></div>';
            }
            if ($row['grade'] > 0) {
                $markup.= '<div> <label>Grade: &nbsp; </label><span>' . $grade . '</span></div>';
            }
            if ($additional_info != '') {
                $markup.= '<div><span>' . $additional_info . '</span></div>';
            }
            if ($row['highest'] == '2') {
                $markup.=$ProgramCodePrev;
            } else {
                $markup.=$ProgramCode;
            }
            $markup.= '</div>
                </div>
                
                </div>
              ';
        }
    }
    $markup.='</div>
            </div> ';
    $markup.= '<div class="pageRow">
                <div class="resume-detail">
                    <div class="resume-detail-title">
                    <div class="re-tit-left"><i class="icon-compass"></i></div>
                    <div class="re-tit-right">Skills</div>
                    </div>';


    $markup.=' <div class="resume-detail-summary">
                <div class="resume-detail-title">';
    foreach ($skillset as $key => $val) {
        $markup.='  
                    <div class="resume-detail-item-left">
                        <div>  ' . $key . ' </div>
                    </div>
                    <div class="resume-detail-item-middle">
                        <div>  ' . implode(', ', $val) . ' </div> 
                    </div>';
    }
    $markup.='  </div>
            </div>';

    $markup.=' </div>
            </div>';
    $markup.= '   <div class="pageRow">
          <div class="resume-detail">
              <div class="resume-detail-title">
              <div class="re-tit-left"><i class="icon-chat"></i></div>
              <div class="re-tit-right">Languages</div>
              </div>
             <div class="resume-detail-summary">
            <div class="resume-detail-title">
            <div class="resume-detail-item-left">Proficiency level: 0 - Poor, 10 - Excellent
            </div>
            <div class="resume-detail-item-middle">
                <div class="resume_languages"> <table border="0">
                    <tr><th width="35%">Language</th>
                        <th width="20%">Spoken</th>
                        <th width="20%">Written</th>
                        <th>Certificate</th></tr>';
    if ($languagesCount > 0) {
        while ($row = $languages->fetchAssoc($languages)) {
            $language = $row['language'];
            $spoken = $row['spoken'];
            $written = $row['written'];
            $certificate = $row['certificate'];
            $markup.=' <tr>
                           <td width="35%"> ' . $language . ' </td>'
                    . '<td width="20%">' . $spoken . '</td>'
                    . '<td width="20%">' . $written . '</td>'
                    . '<td>' . $certificate . '</td>
                        </tr>';
        }
    }
    $markup.= '</table> </div></div></div>
            </div>
            </div></div>';

    $markup.= ' <div class="pageRow">
                <div class="resume-detail">					
                    <div class="resume-detail-title">
                    <div class="re-tit-left"><i class="icon-additional"></i></div>
                    <div class="re-tit-right">Additional Info</div></div>';
    $student_details = db_select('student_details')
            ->fields('student_details')
            ->condition('uid', $user->uid, '=')
            ->execute();
    if ($detcount > 0) {
        while ($row = $student_details->fetchAssoc($student_details)) {
            $expected_salary = $row['expected_salary'];
            $preferred_country = get_country_name($row['location_country']);
            $preferred_state = get_state_name($row['location_state']);
            $other_info = $row['additional_info'];
            $markup.=' <div class="resume-detail-summary">
            <div class="resume-detail-title">
            <div class="resume-detail-item-left">
              &nbsp;
            </div>
            <div class="resume-detail-item-middle">
            <div> Expected Monthly Salary (MYR) : ' . $expected_salary . '</div>
            <div> Preferred Work Location: ' . $preferred_country . ' ' . $preferred_state . '</div>
            <div> Other Information: ' . $other_info . '</div>
            </div>
            </div></div>';
        }
    }

    $markup.= ' </div>
            </div>';
    $markup.= '  <div class="pageRow">
                <div class="resume-detail">					
                <div class="resume-detail-title">
                <div class="re-tit-left"><i class="icon-user"></i></div>
                <div class="re-tit-right">About Me</div></div>					
                <div class="resume-detail-summary border-none">
                <div class="resume-detail-title">
                <div class="resume-detail-item-left">&nbsp;</div>
                <div class="resume-detail-item-middle">
                <div>
                           
                           <div>  Student’s identity card or passport number: ' . $stdinfo['ic_no'] . '</div>
                           <div>  Current level/year: ' . $stdinfo['year'] . '  </div>   
                           
                           <div>  Nationality: ' . $states_list[$stdinfo['ctzship']] . '</div>
                           
                           <div>  Date of birth: ' . $stdinfo['birth_date'] . '</div>
                           <div>  Ethnic origin code: ' . $stdinfo['origin_code'] . '</div>
                           <div>  Bumiputra code: ' . $stdinfo['bumi'] . '</div>
                           <div>  State residential code: ' . $stdinfo['st_rsdc'] . '</div>
                           <div>  Gender code: ' . $stdinfo['gender'] . '
                           <div>  Phone number: ' . $stdinfo['contact_phone'] . '    </div>
                           <div>  Mobile Phone Number : ' . $stdinfo['contact_mobile'] . '</div>
                             
                            <div class="mailing_address">
                            <h2>Mailing Address</h2>
                            <ul class="employer-list">     
                            <li><label>Line 1:</label> <div class="employer-cont">' . $stdinfo['post_add1'] . '</div></li>
                            <li><label>Line 2:</label> <div class="employer-cont">' . $stdinfo['post_add2'] . '</div></li>     
                            <li><label>Line 3:</label> <div class="employer-cont">' . $stdinfo['post_add3'] . '</div></li>
                            </ul>
                            </div>
                            <div class="permanent_address">
                            <h2>Permanent Location</h2>
                            <ul class="employer-list">
                            <li><label>line 1 :</label> <div class="employer-cont">' . $stdinfo['add1'] . '</div></li>
                            <li><label>line 2 :</label> <div class="employer-cont">' . $stdinfo['add2'] . '</div></li>
                            <li><label>line 3 :</label> <div class="employer-cont">' . $stdinfo['add3'] . '</div></li>                
                            Country/State : ' . $country . ' ' . $state . '</div>
                        </div>
                </div> </div>                           
            </div> 
                </div>';

    $markup.= '      </div>
        </div>  </div> ';


    $markup.= '
            <div id="edit-editbar" class="form-item form-type-item">
        <div class="wrapper">
        <ul class="menu">
        <li>
            <a id="nav_experience" href="' . $base_url . '/student/editexperience">
                <div class="experience borderRound" style="behavior: url(http://myjobstreet.jsstatic.com/myjs11-static/default/resume/css/css3.htc)\9;">
                <i rel="icon-briefcase" class="icon-briefcase" id="icon-briefcase"></i>
                </div>
                <div id="nav-lbl-experience">Experience</div>
            </a>
            
        </li>
        <li>
            <a id="nav_education" href="' . $base_url . '/student/editeducation">
            <div class="education borderRound" style="behavior: url(http://myjobstreet.jsstatic.com/myjs11-static/default/resume/css/css3.htc)\9;">
            <i rel="icon-education" class="icon-education" id="icon-education"></i></div><div id="nav-lbl-education">Education</div></a>
             
        </li>
        <li>
            <a id="nav_skills" href="' . $base_url . '/student/editskills">
            <div class="skills borderRound" style="behavior: url(http://myjobstreet.jsstatic.com/myjs11-static/default/resume/css/css3.htc)\9;">
            <i rel="icon-compass" class="icon-compass" id="icon-compass"></i></div><div id="nav-lbl-skill">Skills</div></a>
             
        </li>
        <li>
            <a id="nav_languages" href="' . $base_url . '/student/editlanguages">
            <div class="languages borderRound" style="behavior: url(http://myjobstreet.jsstatic.com/myjs11-static/default/resume/css/css3.htc)\9;">
            <i rel="icon-chat" class="icon-chat" id="icon-chat"></i></div><div id="nav-lbl-language">Languages</div></a>
            
        </li>
        <li>
            <a id="nav_add_info" href="' . $base_url . '/student/editadditionalinfo">
            <div class="additional_info borderRound" style="behavior: url(http://myjobstreet.jsstatic.com/myjs11-static/default/resume/css/css3.htc)\9;">
            <i rel="icon-additional" class="icon-additional" id="icon-additional"></i></div><div id="nav-lbl-additional">Additional Info</div></a>
             
        </li>
        <li>
            <a id="nav_about_me" href="' . $base_url . '/student/aboutme">
            <div class="about_me borderRound" style="behavior: url(http://myjobstreet.jsstatic.com/myjs11-static/default/resume/css/css3.htc)\9;">
            <i rel="icon-user" class="icon-user" id="icon-user"></i></div><div id="nav-lbl-about">About Me</div></a>
             
        </li>
        <li>
            <a class="active" id="previewResumeNav" href="' . $base_url . '/student/resumepreview">
            <div class="preview borderRound" style="behavior: url(http://myjobstreet.jsstatic.com/myjs11-static/default/resume/css/css3.htc)\9;">
            <i class="icon-resume" id="icon-resume"></i>
            </div><div id="nav-lbl-preview">Preview Resume</div></a>
        </li>
        </ul>
    </div></div></div>';
    $output = array(
        'title' => array(
            '#type' => 'markup',
            '#markup' => $markup,
        ),
    );
    return $output;
}

