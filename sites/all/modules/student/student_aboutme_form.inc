<?php 
function student_aboutme_form($form, &$form_state) {
    global $user;    
    global $base_url;    
    $states_list = alumni_std_states_list();
    drupal_add_js(drupal_get_path('theme', 'bootstrap_business') .'/js/jquery.validate.js');    
    drupal_add_js(drupal_get_path('theme', 'bootstrap_business') .'/js/jqueryformvalidation.js');
    drupal_add_js(drupal_get_path('module', 'student') .'/js/custom_validations.js');
    drupal_add_css(drupal_get_path('theme', 'bootstrap_business') .'/css/jquery-ui.css');  
    drupal_add_js(drupal_get_path('theme', 'bootstrap_business') .'/js/jquery-ui.js');
       drupal_add_js('jQuery(document).ready(function($) { 
        $("#dob").datepicker({
        yearRange: "1960:2000",
        changeMonth: true,
        changeYear: true,
        showOn: "both",
        buttonImage: "'.file_create_url('public://calendar.gif').'",
        buttonImageOnly: true,
        onSelect: function(selected) {
        }
        });
       
        $(".popup").click(function () {                    
          window.open($(this).attr("href"), "PopupWindow", "width=600,height=600,scrollbars=yes,resizable=no");
          return false;
        });
        });', 'inline');
      
       
       $student_details = db_select('ldap_user_info','ui');
       $student_details->join('users', 'u', 'ui.uid = u.uid');
       $student_details->join('users_roles', 'ur', 'ui.uid = ur.uid');
       $student_details->join('role', 'r', 'r.rid = ur.rid');
        $student_details->fields('ui',array('givenname','type','cn'));
       $student_details->fields('ur',array('rid'));
       $student_details->fields('r',array('name'));
       $student_details->fields('u',array('mail'));
       $student_details->condition('ui.uid', $user->uid, '=');
       $result1 =  $student_details->execute()->fetchAll();   
       
    //while($row = $student_details->fetchAssoc($student_details)){
      foreach($result1 as $row){
        $email = $row->mail;
        //$name = $row->givenname;
        if($row->type=='ug')
            $usertype = 'Undergraduate';
        if($row->type=='pg')
            $usertype = 'Postgraduate';
        $userrole = $row->name;
        $username = $row->cn;
        
    }
   
    $stdinfo = get_stdinfo($user->uid);
    
    $stdinfoarr = stdinfo($user->uid);
    $name = $stdinfoarr['name']; 
    
    $student_details = db_select('student_details')
    ->fields('student_details')    
    ->condition('uid', $user->uid, '=')
    ->execute();  
    
    
    $form = array();
     $form['title'] = array(
        '#type' => 'item', 
        '#markup' => '<h2>About Me</h2>',       
    );      
    $form['aboutme1'] = array(
        '#type' => 'fieldset', 
        //'#title' => t('Edit Aboutme'), 
        '#prefix' => '<div id="aboutme" style="display:block">',
        //'#suffix' => '</div>',
    ); 
    
    
    $form['edit_aboutme'] = array(
        '#id' => 'Editaboutme',
        '#type' => 'fieldset', 
        '#title' => t('<i id="icon-edit" class="icon-edit"></i>'), 
        '#prefix' => '<div id="EditAboutMe" style="cursor: pointer;float:right;width:100px;font-weight:bold;">',
        '#suffix' => '</div>',
        ); 
    $i=0;
    $count = $student_details->rowCount();
    if($count>0) {
        while($row = $student_details->fetchAssoc($student_details)){
//            $firstname = $row['firstname'];
//            $lastname = $row['lastname'];
            $mobile = $row['mobile'];
            $telephone = $row['telephone'];
            //$email = $row['email'];
            $date = explode('-',$row['dob']);
            $dob = $date[1].'/'.$date[2].'/'.$date[0];
            $nationality= $row['nationality'];
            $photo= $row['photo'];
            $country = $row['country'];
            $state = $row['state'];
            $countryName = get_country_name($row['country']);
            $stateName = get_state_name($row['state']);
            if($photo!='')
                $image = ' <img src="'.file_create_url($photo).'"/>';
            else
                $image = '<img src="'.file_create_url('public://uploads/profile_pictures/profile_pic.jpg').'"/>';
             $form['aboutme' . $i]['info'] = array(
            '#type' => 'item',
           // '#title' => t(isset($proficiency) ? $proficiency . ':' : '' . ':'),
            '#markup' => '
                
                <div id="Education1" class="row" >  
                    <div class="col-md-2 resume_left">
                       '.$image.'
                    </div>
                    <div class="col-md-10 resume_mid">
                        <ul>
                            <li><label>Application ID:</label> <div class="employer-cont">' . $user->uid. '</div></li>
                            <li><label>Student’s Name:</label> <div class="employer-cont">' . $name. '</div></li>    
                            <li><label>Username:</label> <div class="employer-cont">' . $username. '</div></li>
                            <li><label>Role:</label> <div class="employer-cont">' . $userrole. '</div></li>
                            <li><label>Type:</label> <div class="employer-cont">' . $usertype. '</div></li>                            
                            <li><label>Student’s Matric number:</label> <div class="employer-cont">' . $stdinfo['matric_no']. '</div></li>
                            <li><label>Student’s identity card or passport number:</label> <div class="employer-cont">' . $stdinfo['ic_no']. '</div></li>
                            <li><label>Current level/year:</label> <div class="employer-cont">' . $stdinfo['year']. '</div></li>     
                            <li><label>Student’s email address :</label> <div class="employer-cont">' . $stdinfo['alt_email']. '</div></li>
                            <li><label>Nationality:</label> <div class="employer-cont">' . $states_list[$stdinfo['ctzship']]. '</div></li>                            
                            <li><label>Date of birth:</label> <div class="employer-cont">' . $stdinfo['birth_date']. '</div></li>
                            <li><label>Ethnic origin code:</label> <div class="employer-cont">' . $stdinfo['origin_code']. '</div></li>
                            <li><label>Bumiputra code:</label> <div class="employer-cont">' . $stdinfo['bumi']. '</div></li>
                            <li><label>State residential code:</label> <div class="employer-cont">' . $stdinfo['st_rsdc']. '</div></li>
                            <li><label>Gender code:</label> <div class="employer-cont">' . $stdinfo['gender']. '</div></li>
                            <li><label>Phone number :</label> <div class="employer-cont">' . $stdinfo['contact_phone']. '</div></li>    
                            <li><label>Mobile Phone Number :</label> <div class="employer-cont">' . $stdinfo['contact_mobile']. '</div></li>
                            </ul>
                            <div class="mailing_address">
                            <h2>Mailing Address</h2>
                            <ul class="employer-list">     
                            <li><label>Line 1:</label> <div class="employer-cont">' . $stdinfo['post_add1']. '</div></li>
                            <li><label>Line 2:</label> <div class="employer-cont">' . $stdinfo['post_add2']. '</div></li>     
                            <li><label>Line 3:</label> <div class="employer-cont">' . $stdinfo['post_add3']. '</div></li>
                            </ul>
                            </div>
                            <div class="permanent_address">
                            <h2>Permanent Address</h2>
                            <ul class="employer-list">
                            <li><label>line 1 :</label> <div class="employer-cont">' . $stdinfo['add1']. '</div></li>
                            <li><label>line 2 :</label> <div class="employer-cont">' . $stdinfo['add2']. '</div></li>
                            <li><label>line 3 :</label> <div class="employer-cont">' . $stdinfo['add3']. '</div></li>
                            </ul>
                            </div>
                            <ul class="employer-list">
                            <li><label>Permanent Location: </label><span>'.$countryName.' '.$stateName.'</span></li>                            
                            
                            
                        </ul>                
                    </div>
                       
                </div>
                
                ',
                );             
//            <li><label>Name:</label> <div class="employer-cont">' . $name. '</div></li>
//            <li><label>Mobile: </label><span>'.$mobile.'</span></li>
//            <li><label>Telephone: </label><span>'.$telephone.'</span></li>
//            <li><label>Email: </label><span>'.$email.'</span></li>
//            <li><label>DOB: </label><span>'.$dob.'</span></li>       
                $i++;
            
        }
    }
     
     
    /*
    
    $form['aboutme1']['photo'] = array(
        '#type' => 'item',
        '#title' => t('  '),
         '#markup' => '<div><img src="'.file_create_url($photo).'"/></div>',  
    );
    
    $form['aboutme1']['firstname'] = array(
        '#type' => 'item',
        '#title' => t('Firstname:'),
         '#markup' => (isset($firstname) ? $firstname : ''),  
    );

    $form['aboutme1']['lastname'] = array(
        '#type' => 'item',
        '#title' => t('Lastname:'),         
        '#markup' => (isset($lastname) ? $lastname : ''),
    );
    
    $form['aboutme1']['mobile'] = array(
         '#type' => 'item',
        '#title' => t('Mobile:'),         
        '#markup' => (isset($mobile) ? $mobile : ''),
    );
    $form['aboutme1']['telephone'] = array(
         '#type' => 'item',
        '#title' => t('Telephone:'),         
        '#markup' => (isset($telephone) ? $telephone : ''),
    );

    $form['aboutme1']['email'] = array(
     '#type' => 'item', 
     '#title' => 'Email:',
      '#markup' => (isset($email) ? $email : ''),  
     );
    
    
    $form['aboutme1']['dob'] = array(
     '#type' => 'item', 
     '#title' => 'DOB:',
      '#markup' => (isset($dob) ? $dob : ''),  
     );
    
    $form['aboutme1']['nationality'] = array(
     '#type' => 'item', 
     '#title' => 'Nationality:',
      '#markup' => (isset($nationality) ? $nationality : ''),  
     );
    
    $form['aboutme1']['country'] = array(
     '#type' => 'item', 
     '#title' => 'Country:',
      '#markup' => (isset($countryName) ? $countryName : ''),  
     );
    
    $form['aboutme1']['state'] = array(
     '#type' => 'item', 
     '#title' => 'State:',
      '#markup' => (isset($stateName) ? $stateName : ''),  
     );
    */
    
    
    $form['aboutme'] = array(
        '#type' => 'fieldset', 
       // '#title' => t('Edit Aboutme'), 
        '#prefix' => '</div><div id="aboutmeDiv" style="display:none">',
        '#suffix' => '</div>',
    ); 
    
   /* $form['aboutme']['title'] = array(
        '#type' => 'item', 
        '#title' => '<h2>Edit Aboutme</h2>', 
        
    ); */
    
    $form['aboutme']['uid'] = array(
        '#id' => 'uid',
        '#type' => 'textfield',
        '#title' => t('Application ID <span class="form-required">*</span>'),
         '#description' => 'this value is our application ID',
        '#attributes'=>array('readonly'=>'readonly'),
         '#default_value' => (isset($user->uid) ? $user->uid: ''),  
    );
    
    $form['aboutme']['name'] = array(
        '#id' => 'name',
        '#type' => 'textfield',
        '#title' => t('Student’s Name <span class="form-required">*</span>'),
        '#description' => 'this value will be coming from existing database',
        '#attributes'=>array('readonly'=>'readonly'),
         '#default_value' => (isset($name) ? $name : ''),  
    );
    
    $form['aboutme']['username'] = array(
        '#id' => 'username',
        '#type' => 'textfield',
        '#title' => t('Username <span class="form-required">*</span>'),
        '#description' => 'this value will be coming from existing database',
        '#attributes'=>array('readonly'=>'readonly'),
         '#default_value' => (isset($username) ? $username : ''),  
    );
    
    $form['aboutme']['role'] = array(
        '#id' => 'role',
        '#type' => 'textfield',
        '#title' => t('Role <span class="form-required">*</span>'),
        '#description' => 'this value will be coming from existing database',
        '#attributes'=>array('readonly'=>'readonly'),
         '#default_value' => (isset($userrole) ? $userrole : ''),  
    );
    
    $form['aboutme']['type'] = array(
        '#id' => 'type',
        '#type' => 'textfield',
        '#title' => t('Type <span class="form-required">*</span>'),
        '#description' => 'this value will be coming from existing database',
        '#attributes'=>array('readonly'=>'readonly'),
         '#default_value' => (isset($usertype) ? $usertype : ''),  
    );
    
    $form['aboutme']['matric_no'] = array(
        '#id' => 'matric_no',
        '#type' => 'textfield',
        '#title' => t('Student’s Matric number <span class="form-required">*</span>'),
        '#description' => 'this value will be coming from existing database',
        '#attributes'=>array('readonly'=>'readonly'),
         '#default_value' => (isset($stdinfo['matric_no']) ? $stdinfo['matric_no'] : ''),  
    );
    
    $form['aboutme']['ic_no'] = array(
        '#id' => 'ic_no',
        '#type' => 'textfield',
        '#title' => t('Student’s identity card or passport number <span class="form-required">*</span>'),
        '#description' => 'this value will be coming from existing database',
        '#attributes'=>array('readonly'=>'readonly'),
         '#default_value' => (isset($stdinfo['ic_no']) ? $stdinfo['ic_no'] : ''),  
    );
    
    $form['aboutme']['year'] = array(
        '#id' => 'year',
        '#type' => 'textfield',
        '#title' => t('Current level/year <span class="form-required">*</span>'),
        '#description' => 'this value will be coming from existing database',
        '#attributes'=>array('readonly'=>'readonly'),
         '#default_value' => (isset($stdinfo['year']) ? $stdinfo['year'] : ''),  
    );
    $form['aboutme']['email'] = array(
        '#id' => 'email',
        '#type' => 'textfield',
        '#title' => t('Student’s email address  <span class="form-required">*</span>'),
        '#description' => 'this value will be coming from existing database',
        '#attributes'=>array('readonly'=>'readonly'),
         '#default_value' => (isset($stdinfo['alt_email']) ? $stdinfo['alt_email'] : ''),  
    ); 
    
    $form['aboutme']['ctzship'] = array(
        '#id' => 'ctzship',
        '#type' => 'textfield',
        '#title' => t('Nationality <span class="form-required">*</span>'),
        '#description' => 'this value will be coming from existing database',
        '#attributes'=>array('readonly'=>'readonly'),
         '#default_value' => (isset($stdinfo['ctzship']) ? $states_list[$stdinfo['ctzship']]: ''),  
    );

    $form['aboutme']['birth_date'] = array(
        '#id' => 'birth_date',
        '#type' => 'textfield',
        '#title' => t('Date of birth <span class="form-required">*</span>'),
        '#description' => 'this value will be coming from existing database',
        '#attributes'=>array('readonly'=>'readonly'),
         '#default_value' => (isset($stdinfo['birth_date']) ? $stdinfo['birth_date'] : ''),  
    );
    $form['aboutme']['origin_code'] = array(
        '#id' => 'origin_code',
        '#type' => 'textfield',
        '#title' => t('Ethnic origin code <span class="form-required">*</span>'),
        '#description' => 'this value will be coming from existing database',
        '#attributes'=>array('readonly'=>'readonly'),
         '#default_value' => (isset($stdinfo['origin_code']) ? $stdinfo['origin_code'] : ''),  
    );
    $form['aboutme']['bumi'] = array(
        '#id' => 'bumi',
        '#type' => 'textfield',
        '#title' => t('Bumiputra code <span class="form-required">*</span>'),
        '#description' => 'this value will be coming from existing database',
        '#attributes'=>array('readonly'=>'readonly'),
         '#default_value' => (isset($stdinfo['bumi']) ? $stdinfo['bumi'] : ''),  
    );
    
    $form['aboutme']['st_rsdc'] = array(
        '#id' => 'st_rsdc',
        '#type' => 'textfield',
        '#title' => t('State residential code <span class="form-required">*</span>'),
        '#description' => 'this value will be coming from existing database',
        '#attributes'=>array('readonly'=>'readonly'),
         '#default_value' => (isset($stdinfo['st_rsdc']) ? $stdinfo['st_rsdc'] : ''),  
    );
    $form['aboutme']['gender'] = array(
        '#id' => 'gender',
        '#type' => 'textfield',
        '#title' => t('Gender code <span class="form-required">*</span>'),
        '#description' => 'this value will be coming from existing database',
        '#attributes'=>array('readonly'=>'readonly'),
         '#default_value' => (isset($stdinfo['gender']) ? $stdinfo['gender'] : ''),  
    );
     
    $form['aboutme']['telephone'] = array(
        '#id' => 'telephone',
        '#type' => 'textfield',
        '#title' => t('Phone number <span class="form-required">*</span>'),
        '#description' => 'this value will be coming from existing database',
        '#attributes'=>array('readonly'=>'readonly'),
         '#default_value' => (isset($stdinfo['contact_phone']) ? $stdinfo['contact_phone'] : ''),  
    );
    
    $form['aboutme']['mobile'] = array(
        '#id' => 'mobile',
        '#type' => 'textfield',
        '#title' => t('Mobile Phone Number <span class="form-required">*</span>'),
        '#description' => 'this value will be coming from existing database',
        '#attributes'=>array('readonly'=>'readonly'),
         '#default_value' => (isset($stdinfo['contact_mobile']) ? $stdinfo['contact_mobile'] : ''),  
    );
    
    $form['aboutme']['mail'] = array(
         '#type' => 'fieldset',
         '#title' => 'Mailing Address'
     );
    $form['aboutme']['post_add1']['mail'] = array(
        '#id' => 'post_add1',
        '#type' => 'textfield',
        '#title' => t('Line 1 <span class="form-required">*</span>'),
        '#description' => 'this value will be coming from existing database',
        '#attributes'=>array('readonly'=>'readonly'),
         '#default_value' => (isset($stdinfo['post_add1']) ? $stdinfo['post_add1'] : ''),  
    );
    $form['aboutme']['post_add2']['mail'] = array(
        '#id' => 'post_add2',
        '#type' => 'textfield',
        '#title' => t('Line 2 <span class="form-required">*</span>'),
        '#description' => 'this value will be coming from existing database',
        '#attributes'=>array('readonly'=>'readonly'),
         '#default_value' => (isset($stdinfo['post_add2']) ? $stdinfo['post_add2'] : ''),  
    );
    $form['aboutme']['post_add3']['mail'] = array(
        '#id' => 'post_add3',
        '#type' => 'textfield',
        '#title' => t('Line 3 <span class="form-required">*</span>'),
        '#description' => 'this value will be coming from existing database',
        '#attributes'=>array('readonly'=>'readonly'),
         '#default_value' => (isset($stdinfo['post_add3']) ? $stdinfo['post_add3'] : ''),  
    );
    $form['aboutme']['permanent'] = array(
         '#type' => 'fieldset',
         '#title' => 'Permanent Address'
     );
    $form['aboutme']['add1']['permanent'] = array(
        '#id' => 'add1',
        '#type' => 'textfield',
        '#title' => t('Line 1 <span class="form-required">*</span>'),
        '#description' => 'this value will be coming from existing database',
        '#attributes'=>array('readonly'=>'readonly'),
         '#default_value' => (isset($stdinfo['add1']) ? $stdinfo['add1'] : ''),  
    );
    $form['aboutme']['add2']['permanent'] = array(
        '#id' => 'add2',
        '#type' => 'textfield',
        '#title' => t('Line 2 <span class="form-required">*</span>'),
        '#description' => 'this value will be coming from existing database',
        '#attributes'=>array('readonly'=>'readonly'),
         '#default_value' => (isset($stdinfo['add2']) ? $stdinfo['add2'] : ''),  
    );
    $form['aboutme']['add3']['permanent'] = array(
        '#id' => 'add3',
        '#type' => 'textfield',
        '#title' => t('Line 3 <span class="form-required">*</span>'),
        '#description' => 'this value will be coming from existing database',
        '#attributes'=>array('readonly'=>'readonly'),
         '#default_value' => (isset($stdinfo['add3']) ? $stdinfo['add3'] : ''),  
    );
     
    $form['aboutme']['country'] = array(
         '#id' => 'country',
        '#type' => 'select',
        '#title' => t('Country <span class="form-required">*</span>'),
        '#options' => array(''=>'Select') + countries_list(),
        '#ajax' => array(
            'event' => 'change',
            'callback' => 'student_aboutme_state_list_ajax',
            'wrapper' => 'state-list-wrapper',
        ),
        '#default_value' => (isset($country) ? $country : ''),
    );
    
    // Wrapper for state dropdown list
    $form['aboutme']['address']['wrapper'] = array(
        '#prefix' => '<div id="state-list-wrapper">',
        '#suffix' => '</div>',
    );
    
    if ((isset($form_state['values']['country']) && $form_state['values']['country'] != "") || $country>0) {        
        $cid = isset($form_state['values']['country'])?$form_state['values']['country']:$country;        
        $state_list = db_select('states_list')
                ->fields('states_list')
                ->condition('cid', $cid, '=')
                ->execute()
                ->fetchAll();
        $state_list_arr[''] = 'Select';
        foreach ($state_list as $value):
            $state_list_arr[$value->id] = $value->name;
        endforeach;
        if(count($state_list) > 0){
            // Province dropdown list
            $form['aboutme']['address']['wrapper']['state'] = array(
                '#id' => 'state',
                '#type' => 'select',
                '#title' => t('State <span class="form-required">*</span>'),
                '#multiple' => false,
                '#options' => $state_list_arr,
                '#default_value' => (isset($state) ? $state : ''),                
            );
        }
    }
    
    
     /*$form['aboutme']['name'] = array(
        '#id' => 'name',
        '#type' => 'textfield',
        '#title' => t('Name <span class="form-required">*</span>'),
        '#description' => 'this value will be coming from existing database',
        '#attributes'=>array('readonly'=>'readonly'),
         '#default_value' => (isset($name) ? $name : ''),  
    );*/
    
    
   /* $form['aboutme']['lastname'] = array(
        '#id' => 'lastname',
        '#type' => 'textfield',
        '#title' => t('Lastname'),
         '#default_value' => (isset($lastname) ? $lastname : ''),  
    );*/
    
    /*$form['aboutme']['dob'] = array(
        '#id' => 'dob',
        '#type' => 'textfield',
        '#attributes' => array('readonly' => 'readonly'),
        '#title' => t('DOB <span class="form-required">*</span>'),
         '#default_value' => (isset($dob) ? $dob : ''),  
    );
    
    $form['aboutme']['mobile'] = array(
        '#id' => 'mobile',
        '#type' => 'textfield',
        '#title' => t('Mobile <span class="form-required">*</span>'),
         '#default_value' => (isset($mobile) ? $mobile : ''),  
    );
    
    
    $form['aboutme']['telephone'] = array(
        '#id' => 'telephone',
        '#type' => 'textfield',
        '#title' => t('Phone <span class="form-required">*</span>'),
         '#default_value' => (isset($telephone) ? $telephone : ''),  
    );
    
    $form['aboutme']['email'] = array(
        '#id' => 'email',
        '#type' => 'textfield',
        '#title' => t('Email <span class="form-required">*</span>'),
         '#default_value' => (isset($email) ? $email : ''),  
    );*/

    
    
    $form['aboutme']['photo'] = array(
        '#id' => 'photo',
        '#title' => 'Upload Profile Picture',       
        '#type' => 'file',
        '#description' => 'Allowed extensions (.png, .jpeg, .gif)',        
        '#prefix' => '<div class="form-item form-type-file form-item-files-company-logo">',
        '#suffix' => (isset($photo) ? '<img src = "'.file_create_url($photo).'" >' : '').'</div>',
    );  
       
         
        $form['aboutme']['submit'] = array(
            '#type' => 'submit',
            '#value' => t('Submit'),   
            '#prefix' => '<div class="submit_clear_but">',
            //'#suffix' => '</div>',
        );
      
        
        $form['aboutme']['cancel'] = array(
        '#type' => 'item', 
        '#markup' => t('<a  class="experience_calcel" href="'.$base_url.'/student/aboutme">Cancel</a>'),        
        '#suffix' => '</div>',
    ); 
        $form['editbar'] = array(
        '#type' => 'item', 
        '#markup' => '
        <div class="wrapper">
        <ul class="menu">
        <li>
            <a id="nav_experience" href="' . $base_url . '/student/editexperience">
                <div class="experience borderRound" >
                <i rel="icon-briefcase" class="icon-briefcase" id="icon-briefcase"></i>
                </div>
                <div id="nav-lbl-experience">Experience</div>
            </a>
            
        </li>
        <li>
            <a id="nav_education" href="'.$base_url.'/student/editeducation">
            <div class="education borderRound" >
            <i rel="icon-education" class="icon-education" id="icon-education"></i></div><div id="nav-lbl-education">Education</div></a>
             
        </li>
        <li>
            <a id="nav_skills" href="'.$base_url.'/student/editskills">
            <div class="skills borderRound" >
            <i rel="icon-compass" class="icon-compass" id="icon-compass"></i></div><div id="nav-lbl-skill">Skills</div></a>
             
        </li>
        <li>
            <a id="nav_languages" href="'.$base_url.'/student/editlanguages">
            <div class="languages borderRound" >
            <i rel="icon-chat" class="icon-chat" id="icon-chat"></i></div><div id="nav-lbl-language">Languages</div></a>
            
        </li>
        <li>
            <a id="nav_add_info" href="'.$base_url.'/student/editadditionalinfo">
            <div class="additional_info borderRound" >
            <i rel="icon-additional" class="icon-additional" id="icon-additional"></i></div><div id="nav-lbl-additional">Additional Info</div></a>
             
        </li>
        <li>
            <a class="active" id="nav_about_me" href="'.$base_url.'/student/aboutme">
            <div class="about_me borderRound" >
            <i rel="icon-user" class="icon-user" id="icon-user"></i></div><div id="nav-lbl-about">About Me</div></a>
             
        </li>
        <li>
            <a id="previewResumeNav" href="'.$base_url.'/student/resumepreview">
            <div class="preview borderRound" >
            <i class="icon-resume" id="icon-resume"></i>
            </div><div id="nav-lbl-preview">Preview Resume</div></a>
        </li>
        </ul>
    </div>'
    );
    $form['#attributes'] = array('enctype' => 'multipart/form-data');
    
    return $form;  
}

function student_aboutme_form_submit($form, &$form_state) {
    global $user;
    extract($form_state['input']); 
     if(!isset($state)){
           $state = 0;
      } 
      
//      $date = explode('/', $dob);
//      $dob = $date[2].'-'.$date[0].'-'.$date[1];
    
if(isset($_FILES["files"]["name"]['photo']) && $_FILES["files"]["name"]['photo']!=''){
    $photo_name = $_FILES["files"]["name"]['photo'];     
    // Move files to destination folder
    $photo_temp = $_FILES["files"]["tmp_name"]['photo'];
    $photo = time().'_'.$photo_name;
    $dest_photo = 'public://uploads/profile_pictures/'.$photo;
    move_uploaded_file($photo_temp, $dest_photo);
    $arr = array(
//            'firstname' => $firstname,
//            'lastname' => $lastname,
//            'dob' => $dob,
//            'mobile' => $mobile,
//            'telephone' => $telephone,
//            'email' => $email,
            'nationality' => $nationality,
            'country' => $country,
	    'state' => $state,
            'photo' => $dest_photo,
          );
}  else {
    $arr = array(
//            'firstname' => $firstname,
//            'lastname' => $lastname,
//            'dob' => $dob,
//            'mobile' => $mobile,
//            'telephone' => $telephone,
//            'email' => $email,
            'nationality' => $nationality,
            'country' => $country,
	    'state' => $state,
          );
}  
        db_update('student_details')
        ->fields($arr)
        ->condition('uid', $user->uid, '=')
         ->execute(); 
        
        audit_log('update', 'About me updated in manage resume.');
}
