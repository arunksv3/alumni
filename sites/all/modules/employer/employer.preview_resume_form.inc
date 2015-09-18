<?php 
function employer_viewresume($sjid,$studentid) {      
    global $base_url;
    $key = variable_get('IIUM_Encryption_Key', NULL);
    $sjid = (int) decrypt($sjid, $key);
    $studentid= (int) decrypt($studentid, $key);
    $std_job_dtls = student_job_details($sjid, $studentid, 'DESC');
    $std_job_exp_info = student_job_experience_info($sjid, $studentid, 'DESC');
    $std_job_lan = student_job_languages($sjid, $studentid, 'DESC');
    $std_job_qual = student_job_qualification($sjid, $studentid, 'DESC');
    $std_job_skills = student_job_skills($sjid, $studentid, 'ASC');
    
    $states_list = alumni_std_states_list();
    $stdinfo = get_stdinfo($studentid);
    $StdProgram = get_stdprogram($studentid);
    $ProgramCode = $StdProgram['program'];
    $ProgramCodePrev = $StdProgram['prev_program'];
    
    $gradeArr = grade_list();
    $applied_dateArr = explode(' ', $std_job_dtls['0']->created_date);
    $update_dateArr = explode(' ', $std_job_dtls['0']->modified_date);
    $applied_date = dateformat($applied_dateArr['0']);
    $updated_date = dateformat($update_dateArr['0']);
    $feildArr = fields_of_study();
    $nature_businessArr = nature_bussiness_arr();
    $proficiencyArr = proficiency();

    $std_pic = array(
        'path' => $std_job_dtls['0']->photo,
        'width' => '150',
        'height' => '130',
    ); 
    
    if ($std_job_dtls['0']->photo != "")
     $image_tag = theme('image', $std_pic);
    else
     $image_tag = theme('image', array('path' => 'public://uploads/profile_pictures/profile_pic.jpg'));
    
   /*$ldapinfo = db_select('ldap_user_info')
            ->fields('ldap_user_info')
            ->condition('uid',$studentid, '=')
            ->execute();
    $row = $ldapinfo->fetchAssoc($ldapinfo);*/
    $row = stdinfo($studentid);
    $givenname = $row['name'];
    
    $experience = ""; 
    
     if ($std_job_dtls['0']->working_year == "")
        $years = 0;
    else
        $years = date_diff(date_create(date('Y-m-d')), date_create($std_job_dtls['0']->working_year . '-' . $std_job_dtls['0']->working_month . '-01'))->y;
    if ($std_job_dtls['0']->working_month == "")
        $months = 0;
    else
        $months = date_diff(date_create(date('Y-m-d')), date_create($std_job_dtls['0']->working_year . '-' . $std_job_dtls['0']->working_month . '-01'))->m;

    if ($std_job_dtls['0']->experience_level == 0) {
        $experience = $years . ' Years ' . $months . ' Months';
        $position = $std_job_exp_info['0']->job_title . ($std_job_exp_info['0']->company_name != "" ? ' at ' . $std_job_exp_info['0']->company_name : '');
    } else if ($std_job_dtls['0']->experience_level == 1) {
        $position = 'Fresh graduate seeking my first job';
    } else {
        $position = 'Student seeking internship or part-time jobs';
    }
       
        $output = ($std_job_dtls['0']->make_your_pitch != "" ? '<div class="pitch"> Applicant Pitch (Stand out from other applicants) ' . $std_job_dtls['0']->make_your_pitch . '</div>' : '') . '</div>
                <div class="resume-preview-container">
                <input value="Print" name="print" onclick="window.print()" type="button" class="print-btn" />  
                <div class="download-resume"><a href="'.$base_url.'/employer/downloadresume/'.encrypt($studentid, variable_get('IIUM_Encryption_Key', NULL)) . '/' . encrypt($sjid, variable_get('IIUM_Encryption_Key', NULL)) . '"> 
                        <i id="preview-icon-download" class="icon-download tool-icon"></i>
                            Download Resume
                </a></div>
                    <div class="resume-preview-container-content">
                        <div class="resume-formSection">
                            <div class="resume-apply-preview">                          
                                <div class="resume-apply-preview-item">
                                <div class="resume-apply-preview-item-inner">
                                <div class="resume-item-left">' . $std_job_dtls['0']->job_title . '</div>
                                <div class="resume-item-right">' . $std_job_dtls['0']->company_name . '</div></div>                                
                            </div>
                        </div>
                    </div>
                </div>
                <div class="resume-formSection">
                    <div class="pageRow">
                        <div class="resume-row-update">
                            <div class="resume-update">Last Updated ' . $updated_date . '</div>
                            </div>
                            <div class="resume-header">
                                <div class="resume-header-left">&nbsp;</div>
                                    <div class="resume-header-top">
                                        <h1>' . $givenname . '</h1>
                                        <h2>' . $position . '</h2>
                                    </div>
                                </div>
                                <div class="resume-header-inner">
                                    <div class="resume-section-top-left">
                                        <div class="resume-photo">' . $image_tag . '
                                            <div class="exp-salary"><h1>Expected Salary (MYR)</h1> <h2>' . $std_job_dtls['0']->expected_salary . '</h2>
                                        </div>
                                    </div>
                                </div> 
                            <div class="resume-section-top">               
                                <div class="resume-section-top-inner">
                                    <ul>' . ($experience != "" ? '<li><label>Experience:</label> <span>' . $experience . ' </span></li>' : '');
    if (count($std_job_exp_info) > 1) {
        $prev_exp_arr = end($std_job_exp_info);
        $output .= '<li><label>Previous</label> <span>' . $prev_exp_arr->job_title . '
                                                            ' . $prev_exp_arr->company_name . '</span></li>';
    }

    $output .= '<li><label>Education:</label><span>' . $std_job_qual['0']->university .
            ($std_job_qual['0']->graduation_date != "" ? ' (' . date('Y', strtotime($std_job_qual['0']->graduation_date)) . ')' : '') . '  
                                                ' . get_qualiafication_level_name($std_job_qual['0']->qualification_level) . ' 
                                                ' . $std_job_qual['0']->location . '
                                          </span></li>
                                        <li><label>Nationality:</label> <span>' . $states_list[$stdinfo['ctzship']] . '</span></li>
                                        <li><label>Student\'s Matric number:</label> <span>' . $stdinfo['matric_no'] . '</span></li>
                                    </ul>
                                </div>
                            </div>
                            <div class="resume-section-top-right">
                                <div class="pageRow">
                                    <div class="colLeft">
                                        <i class="icon-phone icon icon-align"></i>
                                    </div>
                                    <div class="colMiddle resume-summary-heading"> ' . $std_job_dtls['0']->mobile . '</div>
                                </div>
                                <div class="pageRow">
                                    <div class="colLeft">
                                        <i class="icon-envelope icon"></i>
                                    </div>
                                    <div class="colMiddle resume-summary-heading">' . $std_job_dtls['0']->email . '</div>
                                </div>
                                <div class="pageRow">
                                    <div class="colLeft">
                                        <i class="icon-user icon"></i>
                                    </div>
                                    <div class="colMiddle resume-summary-heading">' . ($stdinfo['birth_date'] != "" ? date_diff(date_create($stdinfo['birth_date']), date_create('today'))->y . ' years old' : '0 years') . '</div>
                                </div>
                                <div class="pageRow">
                                    <div class="colLeft">
                                        <i class="icon-marker-dot icon"></i>
                                    </div>
                                    <div class="colMiddle resume-summary-heading"> ' . get_country_name($std_job_dtls['0']->country) . ' ' . get_state_name($std_job_dtls['0']->state) . '</div></div>
                                </div>
                            </div>
                        </div>	
                        <div class="pageRow">
                            <div class="resume-detail">
                                <div class="resume-detail-title">
                                    <div class="re-tit-left">
                                        <i class="icon-briefcase"></i>
                                    </div>
                                    <div class="re-tit-right">Experience</div></div>
					<div class="resume-detail-summary">
                                            <div class="resume-detail-title">';
    foreach ($std_job_exp_info as $value) {
        $date1 = explode('-', $value->date_joined);
        $date_joined = $date1[1] . '/' . $date1[2] . '/' . $date1[0];
        $date2 = explode('-', $value->date_left);
        if (sizeof($date2) > 1) {
            $date_left = $date2[1] . '/' . $date2[2] . '/' . $date2[0];
        }

        if ($value->present == 1)
            $duration = date_diff(date_create($date_joined), date_create(date('Y-m-d')))->y;
        else
            $duration = date_diff(date_create($date_joined), date_create($date_left))->y;

        $specialization_p = $value->specialization_p;
        $specialization_sp = $value->specialization_sp;

        $db_or = db_or();
        $disp_specialization_records = "";
        $specializations = explode(',', $value->specialization);
        $category_childs = db_select('specializations_c');
        $category_childs->fields('specializations_c', array('sid', 'name'));
        foreach ($specializations as $specialization):
            $db_or->condition('sid', $specialization, '=');
        endforeach;
        $category_childs->condition($db_or);
        $specialization_records = $category_childs->execute()
                ->fetchAll();

        foreach ($specialization_records as $specialization_record):
            $disp_specialization_records .= $specialization_record->name . '<br>';
        endforeach;

        $postionArr = positionoffered();

        if ($value->present == 1)
            $date_duration = date('M Y', strtotime($date_joined)) . ' - Present ';
        else
            $date_duration = date('M Y', strtotime($date_joined)) . ' - ' . date('M Y', strtotime($date_left));

        $output .= '<div class="resume-detail-item-left">
                                                                   ' . $date_duration . '
                                                                    <div>' . $duration . ' Year(s) </div>
                                                                </div>
                                                                <div class="resume-detail-item-middle"> 
                                                                <h1>' . $value->job_title . '</h1>
                                                                <h2>' . $value->company_name . ($value->country != "" ? ' | ' . get_country_name($value->country) . ', ' : '') . get_state_name($value->state) . '</h2>                                                                                                       
                                                                <div>Monthly Salary (MYR): ' . $value->salary . '</div>
                                                                <div>' . $value->description . '</div></div>';
    }
    $output .= '</div></div></div></div>
                        
                                    <div class="pageRow">
                                        <div class="resume-detail">
                                            <div class="resume-detail-title">
                                            <div class="re-tit-left"><i class="icon-education"></i></div>
                                            <div class="re-tit-right">Education</div></div>					
                                            <div class="resume-detail-summary">
                                            <div class="resume-detail-title">';
    foreach ($std_job_qual as $value) {
        $output .= '<div class="resume-detail-item-left">' . date('Y', strtotime($value->graduation_date)) . '</div>
                                                            <div class="resume-detail-item-middle">
                                                            <h1>' . $value->university . '</h1>
                                                            <h2>' . get_qualiafication_name($value->qualification) . ' - ' . get_qualiafication_level_name($value->qualification_level) . '
                                                             in ' . ($value->field_of_study != "" ? $feildArr[$value->field_of_study] : '') . ' | ' . get_country_name($value->location_country) . ' ' . get_state_name($value->location_state) . '</h2>
                                                            <div></div>
                                                            <div>' . ($value->major != "" ? 'Major: ' . $value->major : '') . '</div>
                                                            <div>' . ($value->grade > 0 ? 'Grade: ' . $gradeArr[$value->grade] : '') . '</div> 
                                                            <div>' . ($value->additional_info != "" ? $value->additional_info : '') . '</div> 
                                                            <div>' . ($value->highest == '2' ? $ProgramCodePrev : $ProgramCode) . '</div> 
                                                            </div>';
    }
    $output .= '</div>
                                            </div>
                                        </div>
                                    </div>                        
                                    <div class="pageRow">
					<div class="resume-detail">
                                            <div class="resume-detail-title">
                                            <div class="re-tit-left"><i class="icon-compass"></i></div>
                                            <div class="re-tit-right">Skills</div></div>					
                                            <div class="resume-detail-summary">
                                            <div class="resume-detail-title">';
    foreach ($std_job_skills as $value) {
        $output .= '<div class="resume-detail-item-left">' . ($value->proficiency != "" ? $proficiencyArr[$value->proficiency] : '') . '</div>
                                                            <div class="resume-detail-item-middle">
                                                            <div>' . $value->skills . '</div></div>';
    }
    $output .= '</div></div>
                                        </div>
                                    </div>                        
                                    <div class="pageRow">
					<div class="resume-detail">
                                            <div class="resume-detail-title">
                                            <div class="re-tit-left"><i class="icon-chat"></i></div>
                                            <div class="re-tit-right">Languages</div></div>					
                                            <div class="resume-detail-summary">
                                            <div class="resume-detail-title">
                                            <div class="resume-detail-item-left">Proficiency level: 0 - Poor, 10 - Excellent</div>
                                            <div class="resume-detail-item-middle">
                                            <table border="0">
                                            <tr><th width="35%">Language</th>
                                                <th width="20%">Spoken</th>
                                                <th width="20%">Written</th>
                                                <th>Certificate</th></tr>';
    foreach ($std_job_lan as $value) {
        $output .= '<tr>
                                                                <td width="35%"> ' . $value->language . ' </td>'
                . '<td width="20%">' . $value->spoken . '</td>'
                . '<td width="20%">' . $value->written . '</td>'
                . '<td>' . $value->certificate . '</td>
                                                            </tr>';
    }
    $address = get_country_name($std_job_dtls['0']->country) . ' ' . ($std_job_dtls['0']->state != "0" ? get_state_name($std_job_dtls['0']->state) : '');

    $output .= '</table></div></div>
                                        </div>
                                    </div>                        
                                    <div class="pageRow">
					<div class="resume-detail">					
                                            <div class="resume-detail-title">
                                            <div class="re-tit-left"><i class="icon-additional"></i></div>
                                            <div class="re-tit-right">Additional Info</div></div>
                                            <div class="resume-detail-summary">
                                            <div class="resume-detail-title">
                                            <div class="resume-detail-item-left">&nbsp;</div>
                                            <div class="resume-detail-item-middle">
                                            <div>Expected Monthly Salary (MYR): ' . $std_job_dtls['0']->expected_salary . '</div>
                                            <div>Preferred Work Location:   ' . $std_job_dtls['0']->work_locations . '</div>
                                            <div>Other Information:  ' . $std_job_dtls['0']->additional_info . '</div> </div>                           
                                            </div></div>
                                        </div>
                                    </div>  
                                    <div class="pageRow">
                                        <div class="resume-detail">					
					<div class="resume-detail-title">
					<div class="re-tit-left"><i class="icon-user"></i></div>
					<div class="re-tit-right">About Me</div></div>					
					<div class="resume-detail-summary border-none">
					<div class="resume-detail-title">
                                        <div class="resume-detail-item-left">&nbsp;</div>
                                        <div class="resume-detail-item-middle">
                                        <div>
                                            <div>Student’s identity card or passport number: ' . $stdinfo['ic_no'] . '</div>
                                            <div>Current level/year: ' . $stdinfo['year'] . '</div>   
                                            <div>Nationality: ' . $states_list[$stdinfo['ctzship']] . '</div>
                                            <div>Date of birth: ' . $stdinfo['birth_date'] . '</div>
                                            <div>Ethnic origin code: ' . $stdinfo['origin_code'] . '</div>
                                            <div>Bumiputra code: ' . $stdinfo['bumi'] . '</div>
                                            <div>State residential code: ' . $stdinfo['st_rsdc'] . '</div>
                                            <div>Gender code: ' . $stdinfo['gender'] . '
                                            <div>Phone number: ' . $stdinfo['contact_phone'] . '    </div>
                                            <div>Mobile Phone Number : ' . $stdinfo['contact_mobile'] . '</div>
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
                                            </ul>
                                            Country/State: ' . $address . '
                                        </div>
                                        <div></div>
                                        </div>                            
                                    </div> 
                                </div>
                            </div>
                        </div>';
     
    return $output;
}
/* calcuate age based on dob */
function age($dob) {
    list($year, $month, $day) = explode("-", $dob);
    $year_diff = date("Y") - $year;
    $month_diff = date("m") - $month;
    $day_diff = date("d") - $day;
    if ($day_diff < 0 && $month_diff == 0)
        $year_diff--;
    if ($day_diff < 0 && $month_diff < 0)
        $year_diff--;
    return $year_diff;
}