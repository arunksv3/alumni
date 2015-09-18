<?php

function student_download_resume_form($form, &$form_state, $uid, $jid) {
    $decrypting_key = variable_get('IIUM_Encryption_Key', NULL);
    $uid = (int) decrypt($uid, $decrypting_key);
    $jid = (int) decrypt($jid, $decrypting_key);

    $stdinfo = get_stdinfo($uid);
    $StdProgram = get_stdprogram($uid);
    $ProgramCode = $StdProgram['program'];
    $ProgramCodePrev = $StdProgram['prev_program'];
    $nationality = alumni_std_states_list();

    if ($jid === 01) { // srudent regular resume
        $details_tab = 'student_details';
        $qual_tab = 'student_qualification';
        $exp_tab = 'student_experience';
        $lan_tab = 'student_languages';
        $skill_tab = 'student_skills';
    } else { // srudent job resume
        $details_tab = 'student_job_details';
        $qual_tab = 'student_job_qualification';
        $exp_tab = 'student_job_experience_info';
        $lan_tab = 'student_job_languages';
        $skill_tab = 'student_job_skills';
    }

    $std_qual = db_select($qual_tab)
            ->fields($qual_tab)
            ->condition('uid', $uid, '=');

    if ($jid != '01')
        $std_qual->condition('sjid', $jid, '=');

    $std_job_qual = $std_qual->orderBy('id', 'DESC')
            ->execute()
            ->fetchAll();

    $student_details = db_select($details_tab)
            ->fields($details_tab)
            ->condition('uid', $uid, '=');

    if ($jid != '01')
        $student_details->condition('sjid', $jid, '=');

    $student_details = $student_details->execute();
    $detcount = $student_details->rowCount();


    $student_qualification = db_select($qual_tab)
            ->fields($qual_tab)
            ->condition('uid', $uid, '=');

    if ($jid != '01')
        $student_qualification->condition('sjid', $jid, '=');

    $student_qualification = $student_qualification->orderBy('id', 'desc')
            ->execute();
    $qcount = $student_qualification->rowCount();

    $student_experience1 = db_select($exp_tab)
            ->fields($exp_tab)
            ->condition('uid', $uid, '=');
    if ($jid != '01')
        $student_experience1->condition('sjid', $jid, '=');
    $student_experience1 = $student_experience1->execute();

    $student_experience = db_select($exp_tab)
            ->fields($exp_tab)
            ->condition('uid', $uid, '=');
    if ($jid != '01')
        $student_experience->condition('sjid', $jid, '=');
    $student_experience = $student_experience->execute();

    $ecount = $student_experience1->rowCount();
    $position = '';
    if ($ecount > 0) {
        while ($row = $student_experience1->fetchAssoc($student_experience1)) {
            $position = get_position_name($row['position_level']) . ' at ' . $row['company_name'];
        }
    }

    $student_details1 = db_select($details_tab)
            ->fields($details_tab)
            ->condition('uid', $uid, '=');
    if ($jid != '01')
        $student_details1->condition('sjid', $jid, '=');

    $student_details1 = $student_details1->execute();

    while ($row = $student_details1->fetchAssoc($student_details1)) {
        $experience_level = $row['experience_level'];
        $year = $row['working_year'];
        $month = $row['working_month'];
    }
    $position = '';
    $experience = '';
    if ($experience_level == 0) {
        $student_experience1 = db_select($exp_tab)
                ->fields($exp_tab)
                ->condition('uid', $uid, '=');

        if ($jid != '01')
            $student_experience1->condition('sjid', $jid, '=');

        $student_experience1 = $student_experience1->execute();

        $ecount = $student_experience1->rowCount();

        // if ($ecount > 0) {
        $years = date_diff(date_create(date('Y-m-d')), date_create($year . '-' . $month . '-01'))->y;
        $months = date_diff(date_create(date('Y-m-d')), date_create($year . '-' . $month . '-01'))->m;
        if (!isset($years))
            $years = 0;
        if (!isset($months))
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

    $skills = db_select($skill_tab)
            ->fields($skill_tab)
            ->condition('uid', $uid, '=');
    if ($jid != '01')
        $skills->condition('sjid', $jid, '=');

    $skills = $skills->execute();

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
    $languages = db_select($lan_tab)
            ->fields($lan_tab)
            ->condition('uid', $uid, '=');

    if ($jid != '01')
        $languages->condition('sjid', $jid, '=');

    $languages = $languages->execute();

    $languagesCount = $languages->rowCount();
    $additionalInfo = db_select('student_additional_info')
            ->fields('student_additional_info')
            ->condition('uid', $uid, '=')
            ->execute();
    $additionalInfoCount = $additionalInfo->rowCount();
    /* $ldapinfo = db_select('ldap_user_info')
      ->fields('ldap_user_info')
      ->condition('uid', $uid, '=')
      ->execute();
      while ($row = $ldapinfo->fetchAssoc($ldapinfo)) {
      $givenname = $row['givenname'];
      } */
    $row = stdinfo($uid);
    $givenname = $row['name'];

    $markup.= '<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>Download Resume</title>
<style>
table.languages{border-collapse: collapse; border-color: #ccc; border-width: 1px; color: #333333; font-family: verdana,arial,sans-serif; font-size: 11px;}
table.languages tr{background-color:#e8e8e8;}
table.languages th{background-color:#dddddc; border-color: #ccc; border-style: solid; border-width: 1px; padding: 8px;}
table.languages td{border-color: #ccc; border-style: solid; border-width: 1px; padding: 8px; background:#f7f7f5; font-size:11px;}
</style>
</head>

<body>
<table width="100%" border="0" cellspacing="0" cellpadding="0" style=" width:680px; margin:0 auto; font-family:Arial, Helvetica, sans-serif; font-size:14px;">';
    if ($detcount > 0) {
        while ($row = $student_details->fetchAssoc($student_details)) {
            $firstname = $row['firstname'];
            $lastname = $row['lastname'];
            $email = $row['email'];
            $age = date_diff(date_create($stdinfo['birth_date']), date_create('today'))->y;
//            $date = explode('-', $row['dob']);
//            $dob = $date[1] . '/' . $date[2] . '/' . $date[0];
            $country = get_country_name($row['country']);
            $state = get_state_name($row['state']);            
            $mobile = $row['mobile'];
            $telephone = $row['telephone'];
            $additional_info = $row['additional_info'];
            $photo = $row['photo'];
            if ($photo == '')
                $photo = 'public://uploads/profile_pictures/profile_pic.jpg';
            $searchable = $row['searchable'];
            $experience_in_years = $row['experience_in_years'];
            $experience_in_months = $row['experience_in_months'];
            $expected_salary = $row['expected_salary'];
            $modified_date = date('M d Y H:i:s', strtotime($row['modified_date']));

            $markup.= '<tr>
    <td width="110" align="left" valign="top"><table width="100%" border="0" cellspacing="0" cellpadding="0">
  <tr>
    <td width="86%"><img src="' . drupal_realpath($photo) . '" width="130" height="130" /></td>
    <td width="14%">&nbsp;</td>
  </tr>
  <tr>
    <td align="center" valign="middle" style=" background-color:#333333; font-size:12px; color:#fff; padding:5px;">Expected Salary:</td>
    <td>&nbsp;</td>
  </tr>
  <tr>
    <td align="center" valign="middle" style=" background-color:#333333; font-size:12px; color:#fff; font-weight:600;padding:0 5px 5px;">MYR ' . $expected_salary . '</td>
    <td>&nbsp;</td>
  </tr>
    </table>

</td>
    <td width="570" align="left" valign="top">
    <table width="80%" border="0" cellspacing="0" cellpadding="0">
  <tr>
    <td colspan="2" style="font-size:20px; font-weight:600; padding:0;">' . $givenname . '</td>
    </tr>
  <tr>
    <td colspan="2" style="font-size:16px; font-weight:500; padding:0 0 10px 0;">' . $position . '</td>
    </tr>
  <tr>
    <td colspan="2"><span style="font-size:10px;">' . $mobile . ' | ' . $email . ' | ' . $age . ' Years | ' . $country . ' ' . $state . '</span></td>
    </tr>
  <tr>
    <td width="17%">&nbsp;</td>
    <td width="83%">&nbsp;</td>
  </tr>';
            if ($experience != '') {
                $markup.= '
  <tr>
    <td style="color:#7f7f7f; font-size:12px; padding-bottom:8px;">Experience</td>
    <td style="color:#000; font-size:14px; padding-bottom:8px;">' . $experience . ' </td>
  </tr>
  ';
            }
            $markup.= '
  <tr>
    <td style="color:#7f7f7f; font-size:12px;padding-bottom:8px;">Education</td>
    <td style="color:#000; font-size:14px; padding-bottom:8px;line-height:18px;">' . $std_job_qual['0']->university . ($std_job_qual['0']->graduation_date != "" ? ' (' . date('Y', strtotime($std_job_qual['0']->graduation_date)) . ')' : '') . '
                 <br/>  ' . get_qualiafication_level_name($std_job_qual['0']->qualification_level) . ' 
                 <br/>  ' . $std_job_qual['0']->location . '</td>
  </tr>
  <tr>
    <td style="color:#7f7f7f; font-size:12px;padding-bottom:8px;">Student\'s Matric number </td>
    <td style="color:#000; font-size:14px; padding-bottom:8px;line-height:18px;">' . $stdinfo['matric_no'] . '</td>
  </tr>
    </table>
</td>
  </tr>';
        }
    }

    $ecount = $student_experience->rowCount();
    if ($ecount > 0) {
        while ($row = $student_experience->fetchAssoc($student_experience)) {
            $company_name = $row['company_name'];
            $date1 = explode('-', $row['date_joined']);
            $present = $row['present'];
            $date_joined = $date1[1] . '/' . $date1[2] . '/' . $date1[0];
            if ($row['date_left'] != '') {
                $date2 = explode('-', $row['date_left']);
                $date_left = $date2[1] . '/' . $date2[2] . '/' . $date2[0];
            } else {
                $date_left = '';
            }
            if ($present == 1)
                $duration = date_diff(date_create($date_joined), date_create(date('Y-m-d')))->y;
            else
                $duration = date_diff(date_create($date_joined), date_create($date_left))->y;
            $job_title = $row['job_title'];
            $position = $row['position_level'];
            $location_country = $row['country'];
            $location_state = $row['state'];
            $salary = $row['salary'];
            $description = $row['description'];
            $specializationp = $row['specialization_p'];
            $specializationsp = $row['specialization_sp'];
            $specializationc = $row['specialization_c'];
            //$additional_info = $row['additional_info']; 
            $industry = $row['industry'];
            $type = $row['experience_level'];
            $id = $row['id'];
            $industryArr = nature_bussiness_arr();

            if ($present == 1)
                $dates = date('M Y', strtotime($date_joined)) . ' - Present';
            else
                $dates = date('M Y', strtotime($date_joined)) . ' - ' . date('M Y', strtotime($date_left));


            $markup.= '<tr>
    <td>&nbsp;</td>
    <td>&nbsp;</td>
  </tr>
  <tr>
    <td colspan="2" style=" font-size:16px; font-weight:600; color:#000; padding-bottom:8px;">Experience</td>
  </tr>
  <tr>
    <td align="left" valign="top" style="color:#7f7f7f; font-size:11px; line-height:15px;">' . $dates . '<br />
(' . $duration . ' year )</td>
    <td align="left" valign="middle" style="font-size:15px; color:#000; line-height:18px; padding-bottom:0px; font-weight:600;">
	<table width="75%" border="0" cellspacing="0" cellpadding="0">
  <tr>
    <td>' . $job_title . '</td>
  </tr>
  
  <tr>
   <td align="left" valign="top" style="font-size:13px; color:#000; line-height:16px; padding-bottom:8px; font-weight:normal;">' . $company_name . ' | ' . get_country_name($location_country) . ' ' . get_state_name($location_state) . '</td>
  </tr>
  
  <tr>
    <td align="left" valign="top" style="font-size:13px; color:#000; line-height:16px; padding-bottom:8px; font-weight:normal;">Monthly Salary : ' . $salary . ' MYR</td>
  </tr>
  <tr>
    <td align="left" valign="top" style=" color:#595959; font-size:12px; line-height:18px;font-weight:normal;">' . $description . '</td>
  </tr>
  <tr>
    <td>&nbsp;</td>
  </tr>
</table>
</td>
  </tr>

   ';
        }
    }

    if ($qcount > 0) {
        $gradeArr = grade_list();

        while ($row = $student_qualification->fetchAssoc($student_qualification)) {
            $qualification = $row['qualification'];
            $qualification_level = $row['qualification_level'];
            $field_of_study = $row['field_of_study'];
            $university = $row['university'];
            //$location = $row['location'];
            $location_country = $row['location_country'];
            $location_state = $row['location_state'];
            $graduation_date = date('Y', strtotime($row['graduation_date']));
            $major = $row['major'];
            $grade = $gradeArr[$row['grade']];
            $additional_info = $row['additional_info'];

            $markup.= '<tr>
    <td>&nbsp;</td>
    <td style=" color:#595959; font-size:12px; line-height:18px;">&nbsp;</td>
  </tr>
  <tr>
    <td colspan="2" style=" font-size:16px; font-weight:600; color:#000; padding-bottom:8px;">Education</td>
  </tr>
  <tr>
    <td align="left" valign="top" style="color:#7f7f7f; font-size:11px; line-height:15px;">' . $graduation_date . '</td>
    <td align="left" valign="top" >
	<table width="75%" border="0" cellspacing="0" cellpadding="0">
  <tr>
    <td style="font-size:15px; color:#000; line-height:18px; padding-bottom:0px; font-weight:600;">' . $university . '</td>
  </tr>
  <tr>
    <td align="left" valign="top" style="font-size:13px; color:#000; line-height:16px; padding-bottom:8px; font-weight:normal;">' . get_qualiafication_name($qualification) . ' - ' . get_qualiafication_level_name($qualification_level) . ' in ' . fields_of_study_name($field_of_study) . ' | ' . get_country_name($location_country) . ' ' . get_state_name($location_state) . '</td>
  </tr>
  
  <tr>
    <td align="left" valign="top" ><table width="100%" border="0" cellspacing="0" cellpadding="0" style="padding-bottom:10px;">
  <tr>
    <td width="9%" style="font-size:12px; color:#7f7f7f; padding:3px; font-weight:normal;">Major</td>
    <td width="91%" style="font-size:13px; color:#000; padding:3px; font-weight:normal;">' . $major . '</td>
  </tr>
  <tr>
    <td style="font-size:12px; color:#7f7f7f; padding:3px; font-weight:normal;">Grade</td>
    <td style="font-size:13px; color:#000; padding:3px; font-weight:normal;">' . $grade . '</td>
  </tr>
  
</table></td>
  </tr>
  
  <tr>
    <td align="left" valign="top" style="color:#595959; font-size:12px; line-height:18px;">' . $additional_info . '</td>
  </tr>
  
  <tr>
    <td align="left" valign="top" >&nbsp;</td>
  </tr>
  
  <tr>
 <td align="left" valign="top" >';
            if ($row['highest'] == '2') {
                $markup.=$ProgramCodePrev;
            } else {
                $markup.=$ProgramCode;
            }
            $markup.='</td>
  </tr>
</table>
	</td>
  </tr>
  
  ';
        }
    }

    $markup.= '<tr>
    <td>&nbsp;</td>
    <td align="left" valign="top" >&nbsp;</td>
  </tr>';


    $markup.= '<tr>
    <td colspan="2" style=" font-size:16px; font-weight:600; color:#000; padding-bottom:8px;">Skills</td>
  </tr>
  <tr>
    <td align="left" valign="top"></td>
    <td align="left" valign="top">
    <table width="75%" border="0" cellspacing="0" cellpadding="0">';
    foreach ($skillset as $key => $val) {
        $markup.= '<tr>
    <td width="15%" style="font-size:12px; color:#7f7f7f; padding:3px; font-weight:normal;">' . $key . '</td>
    <td width="60%" style="font-size:13px; color:#000; padding:3px; font-weight:normal;">' . implode(', ', $val) . '</td>
  </tr>';
    }
    $markup.= '</table>
    </td>
  </tr>';


    $markup.= '<tr>
    <td align="left" valign="top">&nbsp;</td>
    <td align="left" valign="top" >&nbsp;</td>
  </tr>
  
  <tr>
    <td>&nbsp;</td>
    <td align="left" valign="top" >&nbsp;</td>
  </tr>';
    if ($languagesCount > 0) {


        $markup.= ' <tr>
    <td colspan="2" style=" font-size:16px; font-weight:600; color:#000; padding-bottom:8px;">Languages</td>
  </tr>
  <tr>
    <td align="left" valign="top" style="color:#7f7f7f; font-size:11px; line-height:15px;">Proficiency level: 0 - Poor, <br />10 -
Excellent</td>
    <td align="left" valign="top" >
    <table width="75%" border="0" cellspacing="0" cellpadding="0" class="languages">
<tr>
<th align="center" valign="middle">Language</th>
<th align="center" valign="middle">Spoken</th>
<th align="center" valign="middle">Written</th>
<th align="center" valign="middle">Certificate</th>
</tr>';
        while ($row = $languages->fetchAssoc($languages)) {
            $language = $row['language'];
            $spoken = $row['spoken'];
            $written = $row['written'];
            $certificate = $row['certificate'];
            $markup.= '<tr>
  <td align="center" valign="middle">' . $language . '</td>
<td align="center" valign="middle">' . $spoken . '</td>
<td align="center" valign="middle">' . $written . '</td>
<td align="center" valign="middle">' . $certificate . '</td></tr>';
        }
        $markup.= ' 
</table></td>
  </tr>';
    }
    $markup.= '<tr>
    <td>&nbsp;</td>
    <td align="left" valign="top" >&nbsp;</td>
  </tr>';

    $student_details = db_select('student_details')
            ->fields('student_details')
            ->condition('uid', $uid, '=')
            ->execute();
    if ($detcount > 0) {
        while ($row = $student_details->fetchAssoc($student_details)) {
            $expected_salary = $row['expected_salary'];
            $preferred_country = get_country_name($row['location_country']);
            $preferred_state = get_state_name($row['location_state']);
            $other_info = $row['additional_info'];



            $markup.='<tr>
    <td colspan="2" style=" font-size:16px; font-weight:600; color:#000; padding-bottom:8px;">Additional Info</td>
  </tr>
  <tr>
    <td>&nbsp;</td>
    <td align="left" valign="top" ><table width="80%" border="0" cellspacing="0" cellpadding="0">
  <tr>
    <td width="28%" style="font-size:12px; color:#7f7f7f; padding:3px; font-weight:normal;">Expected Salary</td>
    <td width="72%" style="font-size:13px; color:#000; padding:3px; font-weight:normal;">MYR ' . $expected_salary . '</td>
  </tr>
  <tr>
    <td style="font-size:12px; color:#7f7f7f; padding:3px; font-weight:normal;">Preferred Work Location</td>
    <td style="font-size:13px; color:#000; padding:3px; font-weight:normal;">' . $preferred_country . ' ' . $preferred_state . '</td>
  </tr>
  <tr>
    <td style="font-size:12px; color:#7f7f7f; padding:3px; font-weight:normal;">Other Information</td>
    <td style="font-size:13px; color:#000; padding:3px; font-weight:normal;">' . $other_info . '</td>
  </tr>
</table>
</td>
  </tr>';
        }
    }


    $markup.= '<tr>
    <td>&nbsp;</td>
    <td align="left" valign="top" >&nbsp;</td>
  </tr>
  <tr>
    <td colspan="2" style=" font-size:16px; font-weight:600; color:#000; padding-bottom:8px;">About Me</td>
  </tr>
  <tr>
    <td>&nbsp;</td>
    <td align="left" valign="top" ><table width="80%" border="0" cellspacing="0" cellpadding="0">
  <tr>
    <td width="28%" style="font-size:12px; color:#7f7f7f; padding:3px; font-weight:normal;">Student\'s identity card or<br />
Passport Number</td>
    <td width="72%" style="font-size:13px; color:#000; padding:3px; font-weight:normal;">' . $stdinfo['ic_no'] . '</td>
  </tr>
  <tr>
    <td style="font-size:12px; color:#7f7f7f; padding:3px; font-weight:normal;">Current level/year</td>
    <td style="font-size:13px; color:#000; padding:3px; font-weight:normal;">' . $stdinfo['year'] . '</td>
  </tr>
  <tr>
    <td style="font-size:12px; color:#7f7f7f; padding:3px; font-weight:normal;">Nationality</td>
    <td style="font-size:13px; color:#000; padding:3px; font-weight:normal;">' . $nationality[$stdinfo['ctzship']] . '</td>
  </tr>
  <tr>
    <td style="font-size:12px; color:#7f7f7f; padding:3px; font-weight:normal;">Date of birth</td>
    <td style="font-size:13px; color:#000; padding:3px; font-weight:normal;">' . $stdinfo['birth_date'] . '</td>
  </tr>
  <tr>
    <td style="font-size:12px; color:#7f7f7f; padding:3px; font-weight:normal;">Ethnic origin code</td>
    <td style="font-size:13px; color:#000; padding:3px; font-weight:normal;">' . $stdinfo['origin_code'] . '</td>
  </tr>
  <tr>
    <td style="font-size:12px; color:#7f7f7f; padding:3px; font-weight:normal;">Bumiputra code</td>
    <td style="font-size:13px; color:#000; padding:3px; font-weight:normal;">' . $stdinfo['bumi'] . '</td>
  </tr>
  <tr>
    <td style="font-size:12px; color:#7f7f7f; padding:3px; font-weight:normal;">State residential code</td>
    <td style="font-size:13px; color:#000; padding:3px; font-weight:normal;">' . $stdinfo['st_rsdc'] . '</td>
  </tr>
  <tr>
    <td style="font-size:12px; color:#7f7f7f; padding:3px; font-weight:normal;">Gender code</td>
    <td style="font-size:13px; color:#000; padding:3px; font-weight:normal;">' . $stdinfo['gender'] . '</td>
  </tr>
  <tr>
    <td style="font-size:12px; color:#7f7f7f; padding:3px; font-weight:normal;">Phone number</td>
    <td style="font-size:13px; color:#000; padding:3px; font-weight:normal;">' . $stdinfo['contact_phone'] . '</td>
  </tr>
  <tr>
    <td style="font-size:12px; color:#7f7f7f; padding:3px; font-weight:normal;">Mobile Phone Number</td>
    <td style="font-size:13px; color:#000; padding:3px; font-weight:normal;">' . $stdinfo['contact_mobile'] . '</td>
  </tr>
  <tr>
    <td style="font-size:12px; color:#7f7f7f; padding:3px; font-weight:normal;">&nbsp;</td>
    <td style="font-size:13px; color:#000; padding:3px; font-weight:normal;">&nbsp;</td>
  </tr>
    </table></td>
  </tr>
  <tr>
    <td>&nbsp;</td>
    <td align="left" valign="top" >&nbsp;</td>
  </tr>
  <tr>
    <td colspan="2" style=" font-size:16px; font-weight:600; color:#000; padding-bottom:8px;">&nbsp;</td>
  </tr>
  <tr>
    <td>&nbsp;</td>
    <td align="left" valign="top" >
	<table width="80%" border="0" cellspacing="0" cellpadding="0">
	<tr>
    <td colspan="2" style="font-size:13px; font-weight:600; color:#000; padding-bottom:8px;">Mailing Address</td>
  </tr>
  <tr>
    <td width="28%">&nbsp;</td>
    <td width="72%" style="font-size:13px; color:#000; padding:3px; font-weight:normal;">' . $stdinfo['post_add1'] . '</td>
  </tr>
  <tr>
    <td>&nbsp;</td>
    <td style="font-size:13px; color:#000; padding:3px; font-weight:normal;">' . $stdinfo['post_add2'] . '</td>
  </tr>
  <tr>
    <td>&nbsp;</td>
    <td style="font-size:13px; color:#000; padding:3px; font-weight:normal;">' . $stdinfo['post_add3'] . '</td>
  </tr>
</table>
</td>
  </tr>
  <tr>
    <td>&nbsp;</td>
    <td align="left" valign="top" >&nbsp;</td>
  </tr>
  <tr>
    <td colspan="2" style=" font-size:16px; font-weight:600; color:#000; padding-bottom:8px;">&nbsp;</td>
  </tr>
  <tr>
    <td>&nbsp;</td>
    <td align="left" valign="top" >
	<table width="80%" border="0" cellspacing="0" cellpadding="0">
	<tr>
    <td colspan="2" style="font-size:13px; font-weight:600; color:#000; padding-bottom:8px;">Permanent Location</td>
  </tr>
  <tr>
    <td width="28%">&nbsp;</td>
    <td width="72%" style="font-size:13px; color:#000; padding:3px; font-weight:normal;">' . $stdinfo['add1'] . '</td>
  </tr>
  <tr>
    <td>&nbsp;</td>
    <td style="font-size:13px; color:#000; padding:3px; font-weight:normal;">' . $stdinfo['add2'] . '</td>
  </tr>
  <tr>
    <td>&nbsp;</td>
    <td style="font-size:13px; color:#000; padding:3px; font-weight:normal;">' . $stdinfo['add3'] . '</td>
  </tr>
</table></td>
  </tr>
  <tr>
    <td>&nbsp;</td>
    <td align="left" valign="top" >&nbsp;</td>
  </tr>
  <tr>
    <td colspan="2" style=" font-size:16px; font-weight:600; color:#000; padding-bottom:8px;">&nbsp;</td>
  </tr>
  <tr>
    <td>&nbsp;</td>
    <td align="left" valign="top" style="font-size:13px; color:#000; padding:3px; font-weight:normal;">Country/State: ' . $country . ' ' . $state . '</td>
  </tr>
  <tr>
    <td>&nbsp;</td>
    <td align="left" valign="top" >&nbsp;</td>
  </tr>
</table>

</body>
</html>';


//echo $markup;exit;
    //require_once("dompdf_config.inc.php");       
    require_once("sites/all/modules/print/lib/dompdf/dompdf_config.inc.php");
//$html = implode('', $markup) ;// you may add your content here
    $dompdf = new DOMPDF;
    $dompdf->load_html($markup);
    $dompdf->render();
// This does not save the pdf field and instead it opens a dialog box asking whether you have to save the pdf or not
    $dompdf->stream("resume.pdf");
}

