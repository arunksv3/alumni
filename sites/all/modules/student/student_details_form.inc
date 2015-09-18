<?php
function student_details_form($form, &$form_state) {
    global $user;
    //echo "<pre>";print_r($user);
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

        $specializationp = 0;
        $specializationsp= 0;
        $specializationc =0;
        $specializationp1 = 0;
        $specializationsp1= 0;
        $specializationc1 =0;
         $country = 0;
         $location_country=0;
        
        $student_details = db_select('student_details')
        ->fields('student_details')    
        ->condition('uid', $user->uid, '=');
        $result =  $student_details->execute()->fetchAll();  
        foreach($result as $row){
            $firstname = $row->firstname;
            $lastname = $row->lastname;
            $email = $row->email;
//            $date = explode('-',$row->dob);
//            $dob = $date[1].'/'.$date[2].'/'.$date[0];
            $country = $row->country;
            $state = $row->state;
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
            //$work_locations = $row->work_locations;
            $location_country = $row->location_country;
            $location_state = $row->location_state;
            $field_of_study = $row->field_of_study;
            //$intrested = $row->intrested;          
            $specializationp = $row->specialization_p;
            $specializationsp= $row->specialization_sp;
            $specializationc = $row->specialization_c;
        }
        
       $student_details = db_select('ldap_user_info','ui');
       $student_details->join('users_roles', 'ur', 'ui.uid = ur.uid');
       $student_details->join('role', 'r', 'r.rid = ur.rid');
        $student_details->fields('ui',array('givenname','type','cn'));
       $student_details->fields('ur',array('rid'));
       $student_details->fields('r',array('name'));
       $student_details->condition('ui.uid', $user->uid, '=');
       $result1 =  $student_details->execute()->fetchAll();   
       
    //while($row = $student_details->fetchAssoc($student_details)){
      foreach($result1 as $row){
    
        $name = $row->givenname;
        $usertype = $row->type;
        $userrole = $row->name;
        $username = $row->cn;
        
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
    
//    echo "<pre>";
//    print_r($user);
//    echo "</pre>";
    $form = array(); 
    $form['title'] = array(
        '#type' => 'item',
         '#markup' => '<h2>Account Details</h2>', 
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
    
   
    
   
     
    $form['additional_info'] = array(
        '#id' => 'additional_info',
        '#title' => 'Additional Info',        
        '#type' => 'text_format',
       // '#required' => TRUE,
        '#default_value' => (isset($additional_info) ? $additional_info : ''),  
       
         
    );
     $form['photo'] = array(
        '#id' => 'photo',
        '#title' => 'Upload Profile Picture ',        
        '#type' => 'file',
        '#description' => 'Allowed extensions (.png, .jpeg, .gif)',        
        '#prefix' => '<div class="form-item form-type-file form-item-files-company-logo">',
        '#suffix' => (isset($photo) ? '<img src = "'.file_create_url($photo).'" >' : '').'</div>',
    ); 
      
        $form['updatemenu'] = array(
        '#id' => 'updatemenu',
        '#type' => 'item', 
        '#prefix' => '<div class="row">',   
        '#suffix' => '</div>',
        );
        
         $form['updatemenu']['student1'] = array(
        '#id' => 'studentDiv',
        '#type' => 'item', 
        '#attributes' => array('class'=>'details_active'),     
        '#prefix' => '<div class="col-md-5 inner-cont details_active">',   
        '#suffix' => '</div>',
        '#markup' => '<img src = "' . file_create_url('sites/default/files/scroll.png') . '" >I am a Student / Fresh Graduate',       
        ); 
         
        $form['updatemenu']['or'] = array(
        '#id' => 'or',
        '#type' => 'item', 
        '#prefix' => '<div class="col-md-2 inner-cont">',   
        '#suffix' => '</div>',
        '#markup' => 'or',       
        ); 
        
        $form['updatemenu']['experience1'] = array(
        '#id' => 'experienceDiv',
        '#type' => 'item', 
        '#prefix' => '<div class="col-md-5 inner-cont">',   
        '#suffix' => '</div>',
        '#markup' => '<img src = "' . file_create_url('sites/default/files/briefcase.png') . '" >I have Work Experience',         
        );
         
        
        
        $form['student'] = array(
         '#type' => 'fieldset', 
        '#prefix' => '<div style="clear:both;">',
        '#suffix' => '</div>',
        );  
        
        $form['experience'] = array(
         '#type' => 'fieldset', 
            '#prefix' => '<div style="clear:both;">',
        '#suffix' => '</div>',
        ); 
 
        
    if(!isset($highest_qualification))
        $highest_qualification = 0;
    if(!isset($highest_qualification_level))
        $highest_qualification_level = 0;
    
    $form['student']['title'] = array(
         '#type' => 'item', 
        '#markup' => '<h2>I am a Student / Fresh Graduate</h2>',
        '#prefix' => '<div id="student" style="display:block">',
        );  
    $form['student']['highest_qualification'] = array(
    '#id' => 'highest_qualification',
    '#type' => 'select',
    '#title' => 'Highest Qualification <span class="form-required">*</span>',
    '#options' =>  array(''=>'Select') +qualification_list(),
    '#default_value' => $highest_qualification, 
    '#ajax' => array(
      // 'event' => 'change',
      'callback' => 'ajax_highest_qualification_level_callback',
      'wrapper' => 'dropdown-highest-degree',
    ),
    
  );
    $selected = isset($form_state['values']['highest_qualification']) ? $form_state['values']['highest_qualification'] : $highest_qualification;
    $form['student']['highest_qualification_level'] = array(
    '#id' => 'highest_qualification_level',
    '#type' => 'select',
    '#title' =>  t('Highest Qualification Level <span class="form-required">*</span>'),
    '#prefix' => '<div id="dropdown-highest-degree">',
    '#suffix' => '</div>',
    '#options' => array(''=>'Select') +highest_qualification_list($selected),
    '#default_value' => isset($form_state['values']['highest_qualification_level']) ? $form_state['values']['highest_qualification_level'] : $highest_qualification_level,
  );  
    
    
        $form['student']['field_of_study'] = array(
        '#id' => 'field_of_study',
        '#title' => 'Field of Study <span class="form-required">*</span>',       
        '#type' => 'select',
        '#options' => fields_of_study(),
      //  '#required' => TRUE,
        '#default_value' => (isset($field_of_study) ? $field_of_study : ''),  
        '#maxlength' => 14,
       
        );
         
  /*      
$form['student']['specializationp'] = array(
    '#type' => 'select',
    '#title' => 'I am Intrested in',
    '#options' =>  array(0=>'Select') + specialization_parrent(),
    '#default_value' => (isset($specializationp1) ? $specializationp1 : ''), 
    '#ajax' => array(
      'callback' => 'ajax_student_specializations_callback',
      'wrapper' => 'dropdown-specializations1',
    ),
    
  );
    $selected = isset($form_state['values']['specializationp']) ? $form_state['values']['specializationp'] : '';
    $form['student']['specializations'] = array(
    '#type' => 'select',
   // '#required' => TRUE,
    '#title' =>  t(' '),
    '#prefix' => '<div id="dropdown-specializations1">',
    '#suffix' => '</div>',
    '#options' => array(0=>'Select') + specialization_subparrent($selected),
    '#default_value' => isset($form_state['values']['specializations']) ? $form_state['values']['specializations'] : '',
    '#ajax' => array(
      'callback' => 'ajax_student_specializationc_callback',
      'wrapper' => 'dropdown-specializationc1',
    ),
  );
    
    
    $selected = isset($form_state['values']['specializations']) ? $form_state['values']['specializations'] : '';
    $form['student']['specializationc'] = array(
    '#type' => 'select',
   // '#required' => TRUE,
    '#title' =>  t(' '),
    '#prefix' => '<div id="dropdown-specializationc1">',
    '#suffix' => '</div>',
    '#options' => array(0=>'Select') + specialization_child($selected),
    '#default_value' => isset($form_state['values']['specializationc']) ? $form_state['values']['specializationc'] : '',
  );
        */
        
    
     $category_parrents = db_select('specializations_p')
                           ->fields('specializations_p') 
                           ->execute()
                           ->fetchAll(); 
    $display_category_parrents['']= 'All Specializations';   
    foreach($category_parrents as $category_parrent_name): 
      $display_category_parrents[$category_parrent_name->sid]= $category_parrent_name->name;   
    endforeach;
    //echo $specializationp;
            
    $form['student']['filter']['specializationp'] = array(
      '#type' => 'select',
    '#id' => 'specializationp',
     //'#default'=>$specializationp,   
      '#title' => t('I am Intrested in <span class="form-required">*</span>'),
      '#options' => $display_category_parrents,
      '#ajax' => array(
       'event'=>'change',
       'callback' => 'student_details_subparrent_list_ajax',
       'wrapper' => 'subparrent-list-wrapper',
     ),
        '#default_value' => $specializationp,
    );
    // Wrapper for sub parent dropdown list
    $form['student']['filter']['wrapper'] = array(
     '#prefix' => '<div id="subparrent-list-wrapper">',
     '#suffix' => '</div>',
    );
    if (isset($form_state['values']['specializationp'])&& $form_state['values']['specializationp']!="" || $specializationp>0) {
        $spid = isset($form_state['values']['specializationp'])?$form_state['values']['specializationp']:$specializationp;
        $category_subparrents = db_select('specializations_sp')
                           ->fields('specializations_sp') 
                           ->condition('spid',$spid, '=')   
                           ->execute()
                           ->fetchAll(); 
        $display_category_subparrents['']= 'Select';   
        foreach($category_subparrents as $category_subparrent_name): 
          $display_category_subparrents[$category_subparrent_name->sid]= $category_subparrent_name->name;   
        endforeach;
         // Province dropdown list
        $form['student']['filter']['wrapper']['specializationsp'] = array(
            '#id' => 'specializationsp',
            '#type' => 'select',
            '#options' => $display_category_subparrents,
            '#ajax' => array(
                'event'=>'change',
                'callback' => 'student_details_child_list_ajax',
                'wrapper' => 'child-list-wrapper',
            ),
            '#default_value' => $specializationsp,
        );         
    }
    // Wrapper for child dropdown list
        $form['student']['filter']['wrapper_child'] = array(
            '#prefix' => '<div id="child-list-wrapper">',
            '#suffix' => '</div>',
         );
        $sub_parent = array();
        if(isset($form_state['values']['specializationsp']) || $specializationsp>0){
            $sub_parent = isset($form_state['values']['specializationsp'])?$form_state['values']['specializationsp']:$specializationsp;
       
       
        if (isset($sub_parent)&& $sub_parent!="") {
            $category_childs = db_select('specializations_c')
                               ->fields('specializations_c') 
                               ->condition('cid',$sub_parent, '=')   
                               ->execute()
                               ->fetchAll(); 
            $display_category_childs['']= 'Select';   
            foreach($category_childs as $category_child_name): 
              $display_category_childs[$category_child_name->sid]= $category_child_name->name;   
            endforeach;
             // Province dropdown list
         $form['student']['filter']['wrapper_child']['specializationc'] = array(
             '#id' => 'specializationc',
            '#type' => 'select',
             '#title' => 'Role<span class="form-required">*</span>',
            '#options' => $display_category_childs,
             '#default_value' => $specializationc,
         );         
        }    
        } 
        
        /*
        $form['student']['intrested'] = array(
        '#id' => 'intrested',
        '#title' => 'I am Intrested in',        
        '#type' => 'textfield',
        //'#required' => TRUE,
        '#default_value' => (isset($intrested) ? $intrested : ''),  
        '#maxlength' => 14,        
        );        
        */
        /*
        $form['student']['nationality'] = array(
        '#id' => 'nationality',
        '#title' => 'Nationality',        
        '#type' => 'select',
        '#options' => array(''=>'Select') + countries_list(),
        //'#required' => TRUE,
        '#default_value' => (isset($nationality) ? $nationality : ''),  
        '#maxlength' => 14
    );*/
        
         $form['student']['country'] = array(
         '#id' => 'country',
        '#type' => 'select',
        '#title' => t('Residing Country<span class="form-required">*</span>'),
        '#options' => array(''=>'Select') + countries_list(),
        '#ajax' => array(
            'event' => 'change',
            'callback' => 'student_state_list_ajax1',
            'wrapper' => 'state-list-wrapper',
        ),
        '#default_value' => (isset($country) ? $country : ''),
    );
    
    // Wrapper for state dropdown list
    $form['student']['wrapper'] = array(
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
            $form['student']['wrapper']['state'] = array(
                '#id' => 'state',
                '#type' => 'select',
                '#title' => t('State<span class="form-required">*</span>'),
                '#multiple' => false,
                '#options' => $state_list_arr,
                '#default_value' => (isset($state) ? $state : ''),                
            );
        }
    }
    
     $form['student']['type'] = array(
        '#type' => 'hidden',
        '#default_value' => 'student',  
        );
     
     $form['student']['location_country'] = array(
         '#id' => 'location_country',
        '#type' => 'select',
        '#title' => t('Intrested at Work Location<span class="form-required">*</span>'),
        '#options' => array(''=>'Select') + countries_list(),
        '#ajax' => array(
            'event' => 'change',
            'callback' => 'student_location_state_list_ajax',
            'wrapper' => 'loction-state-list-wrapper',
        ),
        '#default_value' => (isset($location_country) ? $location_country : ''),
    );
    
    // Wrapper for state dropdown list
    $form['student']['location']['wrapper'] = array(
        '#prefix' => '<div id="loction-state-list-wrapper">',
        '#suffix' => '</div>',
    );
    
    if ((isset($form_state['values']['location_country']) && $form_state['values']['location_country'] != "") || $location_country>0) {        
        $cid = isset($form_state['values']['location_country'])?$form_state['values']['location_country']:$location_country;
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
            $form['student']['location']['wrapper']['location_state'] = array(
                '#id' => 'location_state',
                '#type' => 'select',
                '#title' => t('State<span class="form-required">*</span>'),
                '#multiple' => false,
                '#options' => $state_list_arr,
                '#default_value' => (isset($location_state) ? $location_state: ''),                
            );
        }
    }
     
        /*$form['student']['work_locations'] = array(
        '#id' => 'work_locations',
        '#title' => 'Work Locations',        
        '#type' => 'textfield',
       // '#required' => TRUE,
        '#default_value' => (isset($work_locations) ? $work_locations : ''),  
        '#maxlength' => 14
        );*/
        $form['student']['expected_salary'] = array(
        '#id' => 'expected_salary',
        '#title' => 'Expected Salary (MYR)<span class="form-required">*</span>',        
        '#type' => 'textfield',
        //'#required' => TRUE,
        '#default_value' => (isset($expected_salary) ? $expected_salary : ''),  
        '#maxlength' => 14,
        '#suffix' => '</div>',
        );
        
       
        
        $form['experience']['title'] = array(
         '#type' => 'item', 
        '#markup' => '<h2>I have Work Experience</h2>',
        '#prefix' => '<div id="experience" style="display:none">',
        );
        
        /* $form['experience']['nationality1'] = array(
        '#id' => 'nationality',
        '#title' => 'Nationality',        
        '#type' => 'select',
        '#options' => array(''=>'Select') + countries_list(),
       // '#required' => TRUE,
        '#default_value' => (isset($nationality) ? $nationality : ''),  
        '#maxlength' => 14,
        //'#prefix' => '<div id="experience"  style="display:none">',
    );*/
         
    $form['experience']['country1'] = array(
         '#id' => 'country1',
        '#type' => 'select',
        '#title' => t('Residing Country <span class="form-required">*</span>'),
        '#options' => array(''=>'Select') + countries_list(),
        '#ajax' => array(
            'event' => 'change',
            'callback' => 'student_state_list_ajax2',
            'wrapper' => 'state-list-wrapper2',
        ),
        '#default_value' => (isset($country) ? $country : ''),
    );
    
    // Wrapper for state dropdown list
    $form['experience']['wrapper1'] = array(
        '#prefix' => '<div id="state-list-wrapper2">',
        '#suffix' => '</div>',
    );
    
    if ((isset($form_state['values']['country1']) && $form_state['values']['country1'] != "") || $country>0) {        
        $cid = isset($form_state['values']['country1'])?$form_state['values']['country1']:$country;
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
            $form['experience']['wrapper1']['state1'] = array(
                '#id' => 'state1',
                '#type' => 'select',
                '#title' => t('State <span class="form-required">*</span>'),
                '#multiple' => false,
                '#options' => $state_list_arr,
                '#default_value' => (isset($state) ? $state : ''),                
            );
        }
    }
    
      
     $category_parrents = db_select('specializations_p')
                           ->fields('specializations_p') 
                           ->execute()
                           ->fetchAll(); 
    $display_category_parrents['']= 'All Specializations';   
    foreach($category_parrents as $category_parrent_name): 
      $display_category_parrents[$category_parrent_name->sid]= $category_parrent_name->name;   
    endforeach;
    //echo $specializationp;
            
    $form['experience']['filter']['specializationp1'] = array(
      '#type' => 'select',
    '#id' => 'specializationp1',
     //'#default'=>$specializationp,   
      '#title' => t('I am Intrested in <span class="form-required">*</span>'),
      '#options' => $display_category_parrents,
      '#ajax' => array(
       'event'=>'change',
       'callback' => 'student_details_subparrent_list_ajax1',
       'wrapper' => 'subparrent-list-wrapper1',
     ),
        '#default_value' => $specializationp,
    );
    // Wrapper for sub parent dropdown list
    $form['experience']['filter']['wrapper'] = array(
     '#prefix' => '<div id="subparrent-list-wrapper1">',
     '#suffix' => '</div>',
    );
    if (isset($form_state['values']['specializationp1'])&& $form_state['values']['specializationp1']!="" || $specializationp>0) {
        $spid = isset($form_state['values']['specializationp1'])?$form_state['values']['specializationp1']:$specializationp;
        $category_subparrents = db_select('specializations_sp')
                           ->fields('specializations_sp') 
                           ->condition('spid',$spid, '=')   
                           ->execute()
                           ->fetchAll(); 
        $display_category_subparrents['']= 'Select';   
        foreach($category_subparrents as $category_subparrent_name): 
          $display_category_subparrents[$category_subparrent_name->sid]= $category_subparrent_name->name;   
        endforeach;
         // Province dropdown list
        $form['experience']['filter']['wrapper']['specializationsp1'] = array(
            '#id' => 'specializationsp1',
            '#type' => 'select',
            '#options' => $display_category_subparrents,
            '#ajax' => array(
                'event'=>'change',
                'callback' => 'student_details_child_list_ajax1',
                'wrapper' => 'child-list-wrapper1',
            ),
            '#default_value' => $specializationsp,
        );         
    }
    // Wrapper for child dropdown list
        $form['experience']['filter']['wrapper_child'] = array(
            '#prefix' => '<div id="child-list-wrapper1">',
            '#suffix' => '</div>',
         );
        $sub_parent = array();
        if(isset($form_state['values']['specializationsp1']) || $specializationsp>0){
            $sub_parent = isset($form_state['values']['specializationsp1'])?$form_state['values']['specializationsp1']:$specializationsp;
       
       
        if (isset($sub_parent)&& $sub_parent!="") {
            $category_childs = db_select('specializations_c')
                               ->fields('specializations_c') 
                               ->condition('cid',$sub_parent, '=')   
                               ->execute()
                               ->fetchAll(); 
            $display_category_childs['']= 'Select';   
            foreach($category_childs as $category_child_name): 
              $display_category_childs[$category_child_name->sid]= $category_child_name->name;   
            endforeach;
             // Province dropdown list
         $form['experience']['filter']['wrapper_child']['specializationc1'] = array(
             '#id' => 'specializationc1',
            '#type' => 'select',
             '#title' => 'Role <span class="form-required">*</span>',
            '#options' => $display_category_childs,
             '#default_value' => $specializationc,
         );         
        }
        
        } 
        
        
        /*$form['experience']['intrested1'] = array(
        '#id' => 'intrested',
        '#title' => 'I am Intrested in',        
        '#type' => 'textfield',
      //  '#required' => TRUE,
        '#default_value' => (isset($intrested) ? $intrested : ''),  
        '#maxlength' => 14
        );*/
        
        $form['experience']['location_country1'] = array(
         '#id' => 'location_country1',
        '#type' => 'select',
        '#title' => t('At these Work Location <span class="form-required">*</span>'),
        '#options' => array(''=>'Select') + countries_list(),
        '#ajax' => array(
            'event' => 'change',
            'callback' => 'student_location_state_list_ajax1',
            'wrapper' => 'loction-state-list-wrapper1',
        ),
        '#default_value' => (isset($location_country) ? $location_country : ''),
    );
    
    // Wrapper for state dropdown list
    $form['experience']['location']['wrapper'] = array(
        '#prefix' => '<div id="loction-state-list-wrapper1">',
        '#suffix' => '</div>',
    );
    
    if ((isset($form_state['values']['location_country1']) && $form_state['values']['location_country1'] != "") || $location_country>0) {        
        $cid = isset($form_state['values']['location_country1'])?$form_state['values']['location_country1']:$location_country;
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
            $form['experience']['location']['wrapper']['location_state1'] = array(
                '#id' => 'location_state1',
                '#type' => 'select',
                '#title' => t('State'),
                '#multiple' => false,
                '#options' => $state_list_arr,
                '#default_value' => (isset($location_state) ? $location_state : ''),                
            );
        }
    }
    
        /*$form['experience']['work_locations1'] = array(
        '#id' => 'work_locations',
        '#title' => 'Work Locations',        
        '#type' => 'textfield',
       // '#required' => TRUE,
        '#default_value' => (isset($work_locations) ? $work_locations : ''),  
        '#maxlength' => 14
        );*/
        $form['experience']['exp'] = array(
        '#type' => 'fieldset',
         '#title' => t('Experience - <span>Enter in digits</span>'),
        );
         $form['experience']['exp']['experience_in_years'] = array(
        '#id' => 'experience_in_years',
        '#title' => 'Years <span class="form-required">*</span>',        
        '#type' => 'textfield',
       
        '#default_value' => (isset($experience_in_years) ? $experience_in_years : ''),  
        '#maxlength' => 14
        );
         
          $form['experience']['exp']['experience_in_months'] = array(
        '#id' => 'experience_in_months',
        '#title' => 'Months <span class="form-required">*</span>',        
        '#type' => 'textfield',
        
        '#default_value' => (isset($experience_in_years) ? $experience_in_years : ''),  
        '#maxlength' => 14
        );
          
          
        $form['experience']['type'] = array(              
        '#type' => 'hidden',
        '#default_value' => 'experience',  
        );
          
        $form['experience']['expected_salary1'] = array(
        '#id' => 'expected_salary1',
        '#title' => 'Expected Salary (MYR) <span class="form-required">*</span>',        
        '#type' => 'textfield',
        //'#required' => TRUE,
        '#default_value' => (isset($expected_salary) ? $expected_salary : ''),  
        '#maxlength' => 14,
        '#suffix' => '</div>',
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

/**
 * Validate handler for employer_registration_form_validate().
 */
function student_details_form_validate($form, &$form_state) {   
    
    // file extension validation
    $file_logo = $_FILES["files"]["name"]['photo'];
    $allowed =  array('gif','png' ,'jpg', 'jpeg');
    if($file_logo!=""){
        $logoext = pathinfo($file_logo, PATHINFO_EXTENSION);        
        if(!in_array($logoext, $allowed) ) {
            form_set_error('photo', t('Invalid file extension for profile photo'));
        }
    }
    
     
}

function student_details_form_submit($form, &$form_state) {  
    global $user;
    extract($form_state['input']);
    //echo "<pre>";print_r($form_state['input']);exit;
    
    if(!isset($country1) || $country1=='')
        $country1 = 0;
    if(!isset($country) || $country=='')
        $country = $country1;
    
    if(!isset($state1) || $state1=='')
        $state1 = 0;
    if(!isset($state) || $state=='')
        $state = $state1;
    
    if(!isset($expected_salary1) || $expected_salary1=='')
        $expected_salary1 = 0;
    if(!isset($expected_salary) || $expected_salary=='')
        $expected_salary = 0;    
    if(!isset($experience_in_years) || $experience_in_years=='')
        $experience_in_years = 0;
    if(!isset($experience_in_months) || $experience_in_months=='')
        $experience_in_months = 0;    
    if(!isset($highest_qualification) || $highest_qualification=='')
        $highest_qualification = 0;
    if(!isset($highest_qualification_level) || $highest_qualification_level=='')
        $highest_qualification_level = 0;
    
    if(!isset($specializationp1) || $specializationp1=='')
        $specializationsp1 = 0;
    if(!isset($specializationsp1) || $specializationsp1=='')
        $specializationp1 = 0;
    if(!isset($specializationc1) || $specializationc1=='')
        $specializationc1 = 0;
    
    if(!isset($specializationp) || $specializationp=='')
        $specializationp= $specializationp1;
    if(!isset($specializationsp) || $specializationsp=='')
        $specializationsp= $specializationsp1;
    if(!isset($specializationc) || $specializationc=='')
        $specializationc= $specializationc1;
    
     if(!isset($location_country1) || $location_country1=='')
        $location_country1 = 0;
      if(!isset($location_state1) || $location_state1=='')
        $location_state1 = 0;
    if(!isset($location_country) || $location_country=='')
        $location_country= $location_country1;
    if(!isset($location_state) || $location_state=='')
        $location_state= $location_state1;
    
    if(!isset($nationality) || $nationality=='')
        $nationality= $nationality1;
    
    if(!isset($expected_salary) || $expected_salary=='')
        $expected_salary= $expected_salary1;
 
    $student_details = db_select('student_details')
        ->fields('student_details')    
         ->condition('uid', $user->uid, '=')
         ->execute();  
    $count = $student_details->rowCount(); 
    
    if(isset($_FILES["files"]["name"]['photo']) && $_FILES["files"]["name"]['photo']!=''){   
        $photo_name = $_FILES["files"]["name"]['photo'];     
        // Move files to destination folder
        $photo_temp = $_FILES["files"]["tmp_name"]['photo'];
        $photo = time().'_'.$photo_name;
        $dest_photo = 'public://uploads/profile_pictures/'.$photo;
        move_uploaded_file($photo_temp, $dest_photo);
     } else {
         $dest_photo = '';
     }
//    $date = explode('/', $dob);
//    $dateofbirth = $date[2].'-'.$date[0].'-'.$date[1];
    if($count>0) {
        $arr = array(
	    'email' => $email,
           // 'dob' => $dateofbirth,
            'state' => $state,
            'country' => $country,
            'nationality' => $nationality,
            'mobile' => $mobile,
            'telephone' => $telephone,           
	    'additional_info' => $additional_info['value'],             
            //'work_locations' => $work_locations1,
            'location_country' => $location_country,
            'location_state' => $location_state,
             'expected_salary' => $expected_salary,
            'experience_in_years' => $experience_in_years,
            'experience_in_months' => $experience_in_months,
            'specialization_p' => $specializationp,
            'specialization_sp' => $specializationsp,
            'specialization_c' => $specializationc,
        );
        if(isset($highest_qualification) && $highest_qualification>0){     
              $arr['field_of_study'] =  $field_of_study;
              $arr['highest_qualification'] =  $highest_qualification;
              $arr['highest_qualification_level'] =  $highest_qualification_level;          
        }
        if($dest_photo!=''){
           $arr['photo'] = $dest_photo;
        }     
     
        $eid = db_update('student_details')
        ->fields($arr)
        ->condition('uid', $user->uid, '=')
        ->execute();
    } else{ 
        $arr = array(
            'uid' => $user->uid,
	    'email' => $email,
           // 'dob' => $dateofbirth,
            'state' => $state,
            'country' => $country,
            'nationality' => $nationality,
            'mobile' => $mobile,
            'telephone' => $telephone,
	    'additional_info' => $additional_info['value'],
             //'work_locations' => $work_locations1,
            'location_country' => $location_country,
            'location_state' => $location_state,
             'expected_salary' => $expected_salary,
            'experience_in_years' => $experience_in_years,
            'experience_in_months' => $experience_in_months,  
            'specialization_p' => $specializationp,
            'specialization_sp' => $specializationsp,
            'specialization_c' => $specializationc,
            'created_date' => date('Y-m-d H:i:s')
        );
        
        if(isset($highest_qualification) && $highest_qualification>0){          
            $arr['field_of_study'] = $field_of_study;             
            $arr['highest_qualification'] = $highest_qualification;
            $arr['highest_qualification_level'] = $highest_qualification_level;            
        }
        if($dest_photo!=''){
           $arr['photo'] = $dest_photo;
        }
        // inserting values to employer_registration table
        $eid = db_insert('student_details')
            ->fields($arr)
            ->execute();
    }
    drupal_goto('student/account');
    drupal_set_message(t('details updated.'));    
} 


