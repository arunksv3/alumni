<?php

function student_myaccount_form($form, &$form_state) {
    global $user;
    global $base_url;
    resume_create_validation();
    /* $student_details = db_select('student_details')
      ->fields('student_details')
      ->condition('uid', $user->uid, '=')
      ->execute();
      $count = $student_details->rowCount();
      if($count==0){
      drupal_set_message('Please Create Resume');
      drupal_goto('student/createresume');
      }
     */
    $states_list = alumni_std_states_list();
    $name = '';
    $matric_no = '';
    $ic_no = '';
    $ctzship = '';
    $birth_date = '';
    $origin_code = '';
    $bumi = '';
    $st_rsdc = '';
    $gender = '';
    $contact_phone = '';
    $contact_mobile = '';
    $post_add1 = '';
    $post_add2 = '';
    $post_add3 = '';
    $add1 = '';
    $add2 = '';
    $add3 = '';
    $year = '';
    $alt_email = '';
    $stdinfo = db_select('stdinfo')
            ->fields('stdinfo')
            ->condition('uid', $user->uid, '=');
    $resultstdinfo = $stdinfo->execute()->fetchAll();
    foreach ($resultstdinfo as $val) {
        $name = $val->name ? $val->name : '';
        $matric_no = $val->matric_no ? $val->matric_no : '';
        $ic_no = $val->ic_no ? $val->ic_no : '';
        $ctzship = $val->ctzship ? $val->ctzship : '';
        $birth_date = $val->birth_date ? $val->birth_date : '';
        $origin_code = $val->origin_code ? $val->origin_code : '';
        $bumi = $val->bumi ? $val->bumi : '';
        $st_rsdc = $val->st_rsdc ? $val->st_rsdc : '';
        $gender = $val->gender ? $val->gender : '';
        $contact_phone = $val->contact_phone ? $val->contact_phone : '';
        $contact_mobile = $val->contact_mobile ? $val->contact_mobile : '';
        $post_add1 = $val->post_add1 ? $val->post_add1 : '';
        $post_add2 = $val->post_add2 ? $val->post_add2 : '';
        $post_add3 = $val->post_add3 ? $val->post_add3 : '';
        $add1 = $val->add1 ? $val->add1 : '';
        $add2 = $val->add2 ? $val->add2 : '';
        $add3 = $val->add3 ? $val->add3 : '';
        $year = $val->year ? $val->year : '';
        $alt_email = $val->alt_email ? $val->alt_email : '';
    }


    $student_details = db_select('student_details')
            ->fields('student_details')
            ->condition('uid', $user->uid, '=');
    $result = $student_details->execute()->fetchAll();
    foreach ($result as $row) {
        $firstname = $row->firstname;
        $lastname = $row->lastname;
        //$email = $row->email;
//            $date = explode('-',$row->dob);
//            $dob = $date[1].'/'.$date[2].'/'.$date[0];
        $country = $row->country;
        $state = $row->state;
        $countryName = get_country_name($row->country);
        $stateName = get_state_name($row->state);
        $nationality = $row->nationality;
        $mobile = $row->mobile;
        $telephone = $row->telephone;
        $additional_info = $row->additional_info;
        $photo = $row->photo;
        //$searchable = $row['searchable'];
        $highest_qualification = $row->highest_qualification;
        $highest_qualification_level = $row->highest_qualification_level;
        $experience_in_years = $row->experience_in_years;
        $experience_in_months = $row->experience_in_months;
        $expected_salary = $row->expected_salary;
        $work_locations = $row->work_locations;
        $field_of_study = $row->field_of_study;
        $intrested = $row->intrested;
        $specializationp = $row->specialization_p;
        $specializationsp = $row->specialization_sp;
        $specializationc = $row->specialization_c;
        $modified_date = date('M d Y', strtotime($row->modified_date));
    }

    $student_details = db_select('ldap_user_info', 'ui');
    $student_details->join('users', 'u', 'ui.uid = u.uid');
    $student_details->join('users_roles', 'ur', 'ui.uid = ur.uid');
    $student_details->join('role', 'r', 'r.rid = ur.rid');
    $student_details->fields('ui', array('givenname', 'type', 'cn'));
    $student_details->fields('ur', array('rid'));
    $student_details->fields('r', array('name'));
    $student_details->fields('u', array('mail'));
    $student_details->condition('ui.uid', $user->uid, '=');
    $result1 = $student_details->execute()->fetchAll();

    //while($row = $student_details->fetchAssoc($student_details)){
    foreach ($result1 as $row) {
        $email = $row->mail;
        //$name = $row->givenname;
        if ($row->type == 'ug')
            $usertype = 'Undergraduate';
        if ($row->type == 'pg')
            $usertype = 'Postgraduate';
        $userrole = $row->name;
        $username = $row->cn;
    }
    $experience = '';
    if ($experience_in_years > 0 || $experience_in_months > 0) {
        $experience = '<li><label>Experience:</label> <div class="employer-cont">' . $experience_in_years . ' Years ' . $experience_in_months . ' Months' . '</div></li>';
    }
    $output = array(
        'title' => array(
            '#type' => 'markup',
            '#markup' => '
            <h1>Welcome, ' . $name . '!!</h1>
                
            <div class="myaccount"> 
            <div class="myaccount-tit"><h2>My Account </h2>
            <div class="createbutton"><a href="' . $base_url . '/student/createresume">Create / Update Resume</a></div>
                </div>                                                 
                <div class="myaccount-cont">
                
                    <div class="row" >
                        <div class="col-md-2 upload_photo">' . ($photo != "" ? '<img src = "' . file_create_url($photo) . '" >' : '<img src="' . file_create_url('public://uploads/profile_pictures/profile_pic.jpg') . '"/>') . '
                            <div class="latest_update">Last Updated: ' . $modified_date . '</div>'
            . '
                                
                        </div>
                        <div class="col-md-10 myacount_content">
                            
                            <ul class="employer-list"> 
                            <li><label>Application ID:</label> <div class="employer-cont">' . $user->uid . '</div></li>
                            <li><label>Student’s Name:</label> <div class="employer-cont">' . $name . '</div></li>    
                            <li><label>Username:</label> <div class="employer-cont">' . $username . '</div></li>
                            <li><label>Role:</label> <div class="employer-cont">' . $userrole . '</div></li>
                            <li><label>Type:</label> <div class="employer-cont">' . $usertype . '</div></li>                            
                            <li><label>Student’s Matric number:</label> <div class="employer-cont">' . $matric_no . '</div></li>
                            <li><label>Student’s identity card or passport number:</label> <div class="employer-cont">' . $ic_no . '</div></li>
                            <li><label>Current level/year:</label> <div class="employer-cont">' . $year . '</div></li>     
                            <li><label>Student’s email address :</label> <div class="employer-cont">' . $alt_email . '</div></li>
                            <li><label>Nationality: </label><span>' . $states_list[$ctzship] . '</span></li>                                
                            <li><label>Date of birth:</label> <div class="employer-cont">' . $birth_date . '</div></li>
                            <li><label>Ethnic origin code:</label> <div class="employer-cont">' . $origin_code . '</div></li>
                            <li><label>Bumiputra code:</label> <div class="employer-cont">' . $bumi . '</div></li>
                            <li><label>State residential code:</label> <div class="employer-cont">' . $st_rsdc . '</div></li>
                            <li><label>Gender code:</label> <div class="employer-cont">' . $gender . '</div></li>
                            <li><label>Phone number :</label> <div class="employer-cont">' . $contact_phone . '</div></li>    
                            <li><label>Mobile Phone Number :</label> <div class="employer-cont">' . $contact_mobile . '</div></li>
                            </ul>
                            <div class="mailing_address">
                            <h2>Mailing Address</h2>
                            <ul class="employer-list"> 
                            <li><label>Line 1:</label> <div class="employer-cont">' . $post_add1 . '</div></li>
                            <li><label>Line 2:</label> <div class="employer-cont">' . $post_add2 . '</div></li>     
                            <li><label>Line 3:</label> <div class="employer-cont">' . $post_add3 . '</div></li>
                            </ul>
                            </div>
                            <div class="permanent_address">
                            <h2>Permanent Address</h2>
                            <ul class="employer-list">
                            <li><label>line 1 :</label> <div class="employer-cont">' . $add1 . '</div></li>
                            <li><label>line 2 :</label> <div class="employer-cont">' . $add2 . '</div></li>
                            <li><label>line 3 :</label> <div class="employer-cont">' . $add3 . '</div></li>
                            </ul>
                            </div>
                            <ul class="employer-list">
                            <li><label>Permanent Location: </label><span>' . $countryName . ' ' . $stateName . '</span></li>                            
                            
                                ' . $experience . '
                                <li><label>Additional Info:</label> <div class="employer-cont">' . $additional_info . '</div></li>
                            </ul>
                        </div>
                    </div>
                </div>                            
            </div>',
        ),
    );
    return $output;
}
