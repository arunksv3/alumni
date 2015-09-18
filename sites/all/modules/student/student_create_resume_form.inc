<?php
function student_create_resume_form($form, &$form_state) {
    global $user;
    //validating ccreate resume details
    $states_list = alumni_std_states_list();
    resume_create_validation();
    manage_resume_validation();
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

    
     $student_details = db_select('student_details','s');
     $student_details ->fields('s');  
     $student_details  ->condition('s.uid', $user->uid, '=');
     $result =  $student_details->execute()->fetchAll();   
    foreach($result as $row){
//        $name = $row->givenname;
//        $email = $row->email;
//        $date = explode('-',$row->dob);
//        $dob = $date[1].'/'.$date[2].'/'.$date[0];
        $country = $row->country;
        $state = $row->state;
        $nationality = $row->nationality;        
        $mobile = $row->mobile;         
        $telephone = $row->telephone;
        $searchable = $row->searchable;
    }
    
    
    $student_details = db_select('users','u');
    $student_details->join('ldap_user_info', 'ui', 'u.uid = ui.uid');
    $student_details ->fields('u',array('mail'));  
    $student_details->fields('ui',array('givenname','type','cn'));
    $student_details  ->condition('u.uid', $user->uid, '=');
     $result =  $student_details->execute()->fetchAll();   
    foreach($result as $row){
        $name = $row->givenname;
        $email = $row->mail;         
    }
    
    $name = ''; $matric_no = ''; $ic_no = ''; $ctzship = '';
    $birth_date = ''; $origin_code = ''; $bumi = ''; $st_rsdc = '';
    $gender = ''; $contact_phone = ''; $contact_mobile = '';
    $post_add1 = ''; $post_add2 = ''; $post_add3 = '';
    $add1 = ''; $add2 = ''; $add3 = '';
    $year = ''; $alt_email = '';
    $stdinfo = db_select('stdinfo')
    ->fields('stdinfo')    
    ->condition('uid', $user->uid, '=');
    $resultstdinfo =  $stdinfo->execute()->fetchAll();  
    foreach($resultstdinfo as $val){
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
    
    $form = array(); 
    
    $form['title'] = array(
        '#type' => 'item',        
        '#markup' => '<h2>Create / Manage Resume</h2>',       
    );
    // To make these fields required
   
    $form['uid'] = array(
        '#id' => 'uid',
        '#type' => 'textfield',
        '#title' => t('Application ID <span class="form-required">*</span>'),
         '#description' => 'this value is our application ID',
        '#attributes'=>array('readonly'=>'readonly'),
         '#default_value' => (isset($user->uid) ? $user->uid: ''),  
    );
    
    $form['name'] = array(
        '#id' => 'name',
        '#type' => 'textfield',
        '#title' => t('Student’s Name <span class="form-required">*</span>'),
        '#description' => 'this value will be coming from existing database',
        '#attributes'=>array('readonly'=>'readonly'),
         '#default_value' => (isset($name) ? $name : ''),  
    );
    
    $form['username'] = array(
        '#id' => 'username',
        '#type' => 'textfield',
        '#title' => t('Username <span class="form-required">*</span>'),
        '#description' => 'this value will be coming from existing database',
        '#attributes'=>array('readonly'=>'readonly'),
         '#default_value' => (isset($username) ? $username : ''),  
    );
    
    $form['role'] = array(
        '#id' => 'role',
        '#type' => 'textfield',
        '#title' => t('Role <span class="form-required">*</span>'),
        '#description' => 'this value will be coming from existing database',
        '#attributes'=>array('readonly'=>'readonly'),
         '#default_value' => (isset($userrole) ? $userrole : ''),  
    );
    
    $form['type'] = array(
        '#id' => 'type',
        '#type' => 'textfield',
        '#title' => t('Type <span class="form-required">*</span>'),
        '#description' => 'this value will be coming from existing database',
        '#attributes'=>array('readonly'=>'readonly'),
         '#default_value' => (isset($usertype) ? $usertype : ''),  
    );
    
    $form['matric_no'] = array(
        '#id' => 'matric_no',
        '#type' => 'textfield',
        '#title' => t('Student’s Matric number <span class="form-required">*</span>'),
        '#description' => 'this value will be coming from existing database',
        '#attributes'=>array('readonly'=>'readonly'),
         '#default_value' => (isset($matric_no) ? $matric_no : ''),  
    );
    
    $form['ic_no'] = array(
        '#id' => 'ic_no',
        '#type' => 'textfield',
        '#title' => t('Student’s identity card or passport number <span class="form-required">*</span>'),
        '#description' => 'this value will be coming from existing database',
        '#attributes'=>array('readonly'=>'readonly'),
         '#default_value' => (isset($ic_no) ? $ic_no : ''),  
    );
    
    $form['year'] = array(
        '#id' => 'year',
        '#type' => 'textfield',
        '#title' => t('Current level/year <span class="form-required">*</span>'),
        '#description' => 'this value will be coming from existing database',
        '#attributes'=>array('readonly'=>'readonly'),
         '#default_value' => (isset($year) ? $year : ''),  
    );
    $form['email'] = array(
        '#id' => 'email',
        '#type' => 'textfield',
        '#title' => t('Student’s email address  <span class="form-required">*</span>'),
        '#description' => 'this value will be coming from existing database',
        '#attributes'=>array('readonly'=>'readonly'),
         '#default_value' => (isset($alt_email) ? $alt_email : ''),  
    ); 
    
    $form['ctzship'] = array(
        '#id' => 'ctzship',
        '#type' => 'textfield',
        '#title' => t('Nationality <span class="form-required">*</span>'),
        '#description' => 'this value will be coming from existing database',
        '#attributes'=>array('readonly'=>'readonly'),
        '#default_value' => (isset($ctzship) ? $states_list[$ctzship] : ''),  
    );
    
    $form['birth_date'] = array(
        '#id' => 'birth_date',
        '#type' => 'textfield',
        '#title' => t('Date of birth <span class="form-required">*</span>'),
        '#description' => 'this value will be coming from existing database',
        '#attributes'=>array('readonly'=>'readonly'),
         '#default_value' => (isset($birth_date) ? $birth_date : ''),  
    );
    $form['origin_code'] = array(
        '#id' => 'origin_code',
        '#type' => 'textfield',
        '#title' => t('Ethnic origin code <span class="form-required">*</span>'),
        '#description' => 'this value will be coming from existing database',
        '#attributes'=>array('readonly'=>'readonly'),
         '#default_value' => (isset($origin_code) ? $origin_code : ''),  
    );
    $form['bumi'] = array(
        '#id' => 'bumi',
        '#type' => 'textfield',
        '#title' => t('Bumiputra code <span class="form-required">*</span>'),
        '#description' => 'this value will be coming from existing database',
        '#attributes'=>array('readonly'=>'readonly'),
         '#default_value' => (isset($bumi) ? $bumi : ''),  
    );
    
    $form['st_rsdc'] = array(
        '#id' => 'st_rsdc',
        '#type' => 'textfield',
        '#title' => t('State residential code <span class="form-required">*</span>'),
        '#description' => 'this value will be coming from existing database',
        '#attributes'=>array('readonly'=>'readonly'),
         '#default_value' => (isset($st_rsdc) ? $st_rsdc : ''),  
    );
    $form['gender'] = array(
        '#id' => 'gender',
        '#type' => 'textfield',
        '#title' => t('Gender code <span class="form-required">*</span>'),
        '#description' => 'this value will be coming from existing database',
        '#attributes'=>array('readonly'=>'readonly'),
         '#default_value' => (isset($gender) ? $gender : ''),  
    );
     
    $form['telephone'] = array(
        '#id' => 'telephone',
        '#type' => 'textfield',
        '#title' => t('Phone number <span class="form-required">*</span>'),
        '#description' => 'this value will be coming from existing database',
        '#attributes'=>array('readonly'=>'readonly'),
         '#default_value' => (isset($contact_phone) ? $contact_phone : ''),  
    );
    
    $form['mobile'] = array(
        '#id' => 'mobile',
        '#type' => 'textfield',
        '#title' => t('Mobile Phone Number <span class="form-required">*</span>'),
        '#description' => 'this value will be coming from existing database',
        '#attributes'=>array('readonly'=>'readonly'),
         '#default_value' => (isset($contact_mobile) ? $contact_mobile : ''),  
    );
    
     $form['mail'] = array(
         '#type' => 'fieldset',
         '#title' => 'Mailing Address'
     );
    $form['mail']['post_add1'] = array(
        '#id' => 'post_add1',
        '#type' => 'textfield',
        '#title' => t('Line 1 <span class="form-required">*</span>'),
        '#description' => 'this value will be coming from existing database',
        '#attributes'=>array('readonly'=>'readonly'),
         '#default_value' => (isset($post_add1) ? $post_add1 : ''),  
    );
    $form['mail']['post_add2'] = array(
        '#id' => 'post_add2',
        '#type' => 'textfield',
        '#title' => t('Line 2 <span class="form-required">*</span>'),
        '#description' => 'this value will be coming from existing database',
        '#attributes'=>array('readonly'=>'readonly'),
         '#default_value' => (isset($post_add2) ? $post_add2 : ''),  
    );
    $form['mail']['post_add3'] = array(
        '#id' => 'post_add3',
        '#type' => 'textfield',
        '#title' => t('Line 3 <span class="form-required">*</span>'),
        '#description' => 'this value will be coming from existing database',
        '#attributes'=>array('readonly'=>'readonly'),
         '#default_value' => (isset($post_add3) ? $post_add3 : ''),  
    );
    
    $form['permanent'] = array(
         '#type' => 'fieldset',
         '#title' => 'Permanent Address'
     );
    
    $form['permanent']['add1'] = array(
        '#id' => 'add1',
        '#type' => 'textfield',
        '#title' => t('Line 1 <span class="form-required">*</span>'),
        '#description' => 'this value will be coming from existing database',
        '#attributes'=>array('readonly'=>'readonly'),
         '#default_value' => (isset($add1) ? $add1 : ''),  
    );
    $form['permanent']['add2'] = array(
        '#id' => 'add2',
        '#type' => 'textfield',
        '#title' => t('Line 2 <span class="form-required">*</span>'),
        '#description' => 'this value will be coming from existing database',
        '#attributes'=>array('readonly'=>'readonly'),
         '#default_value' => (isset($add2) ? $add2 : ''),  
    );
    $form['permanent']['add3'] = array(
        '#id' => 'add3',
        '#type' => 'textfield',
        '#title' => t('Line 3 <span class="form-required">*</span>'),
        '#description' => 'this value will be coming from existing database',
        '#attributes'=>array('readonly'=>'readonly'),
         '#default_value' => (isset($add3) ? $add3 : ''),  
    );
     
      /*  $form['student']['nationality'] = array(
        '#id' => 'nationality',
        '#title' => 'Nationality <span class="form-required">*</span>',        
        '#type' => 'select',
        '#options' => array(''=>'Select') + countries_list(),
        //'#required' => TRUE,
        '#default_value' => (isset($nationality) ? $nationality : ''),  
        '#maxlength' => 14
    );*/
        
         $form['student']['country'] = array(
         '#id' => 'country',
        '#type' => 'select',
        '#title' => t('Country <span class="form-required">*</span>'),
        '#options' => array(''=>'Select') + countries_list(),
        '#ajax' => array(
            'event' => 'change',
            'callback' => 'employer_registration_state_list_ajax',
            'wrapper' => 'state-list-wrapper',
        ),
        '#default_value' => (isset($country) ? $country : ''),
    );
    
    // Wrapper for state dropdown list
    $form['address']['wrapper'] = array(
        '#prefix' => '<div id="state-list-wrapper">',
        '#suffix' => '</div>',
    );
    
    if ((isset($form_state['values']['country']) && $form_state['values']['country'] != "") || (isset($country) && $country>0) ) {
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
            $form['address']['wrapper']['state'] = array(
                '#id' => 'state',
                '#type' => 'select',
                '#title' => t('State <span class="form-required">*</span>'),
                '#multiple' => false,
                '#options' => $state_list_arr,
                '#default_value' => (isset($state) ? $state : ''),                
            );
        }
    }
     
     $form['title1'] = array(
            '#type' => 'item',
            '#markup' => '<h2>Create My Resume</h2>'
         );
     $form['resume1'] = array(
            '#id' => 'resume',
            '#type' => 'radios',
            '#title' => t(''), 
            '#options' => array(1 => t('Upload My Own Resume '), 0 => t('Build a IIUM Resume')),
            //'#required' => TRUE,
            '#default_value' => 0,
            
    );
     
    $form['privacy'] = array(
        '#type' => 'fieldset', 
        '#title' => t(' '), 
        '#prefix' => '<div id="privacyDiv"  style="display:none">',
        '#suffix' => '</div>',
    );  
     
     $form['privacy']['resume'] = array(
            '#id' => 'resume',
            '#type' => 'file',
            //'#required' => TRUE,
            '#description'=>'Upload document of types docx, doc, pdf.',
            '#title' => t('Resume <span class="form-required">*</span>')
        );  
     
     $form['privacy']['title'] = array(
             '#type' => 'item',
            '#markup' => '<h2>Privacy Setting</h2>',            
        );
     
     $form['privacy']['searchable'] = array(
            '#id' => 'searchable',
            '#type' => 'radios',            
            '#options' => array(
                1 => t('Searchable with Contact Details <span class="note_privacy">Allow employers to search for my resume and see my name and contact details.</span>'), 
                0 => t('Not Searchable <span class="note_privacy">Do not allow employers to search for my resume.</span>')
            ),
            //'#required' => TRUE,
            '#default_value' => (isset($searchable) ? $searchable : ''),
           // '#weight' => 1,
            //'#prefix' => '<div>',
           // '#suffix' => '</div>',
        );
     
     
    $form['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Submit'),   
        '#prefix' => '<div>',
        '#suffix' => '</div>',
    );

    
    $form['#attributes'] = array('enctype' => 'multipart/form-data');
    
    return $form;       
} 

function student_create_resume_form_submit($form, &$form_state) {  
    global $user;
    extract($form_state['input']);
   
    if(!isset($state) || $state =='')
        $state = 0;        
    
//    $date = explode('/', $dob);
//    $dateofbirth = $date[2].'-'.$date[0].'-'.$date[1];
  
        $arr = array(
//            'firstname' => $firstname,
//            'lastname' => $lastname,
	    'email' => $email,
            'dob' => date('Y-m-d'),
            'state' => $state,
            'country' => $country,
            'nationality' => $nationality,
            'mobile' => $mobile,
            'telephone' => $telephone,            
         );
    
        db_update('student_details')
        ->fields($arr)
        ->condition('uid', $user->uid, '=')
        ->execute();
 
    if(isset($_FILES["files"]["name"]['resume']) && $_FILES["files"]["name"]['resume']!='') {
        $resume = $_FILES["files"]["name"]['resume'];    
        $resume_temp = $_FILES["files"]["tmp_name"]['resume'];
        $resume = time().'_'.$resume;
        $dest_resume = 'public://uploads/resumes/'.$resume;
        move_uploaded_file($resume_temp, $dest_resume);
        db_update('student_details')
            ->fields(
                array(
                    'searchable' => $searchable,
                    'resume' => $dest_resume
                )
            )
            ->condition('uid', $user->uid, '=')
            ->execute();
        audit_log('update', 'Resume Uploaded.');
        drupal_goto('student/editexperience');
    } else {
        drupal_goto('student/uploadresume');
    }
    
    drupal_set_message(t('details updated.'));    
} 


