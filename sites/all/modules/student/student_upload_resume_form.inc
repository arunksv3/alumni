<?php 
function student_upload_resume_form($form, &$form_state) {
    global $user;
    drupal_add_js(drupal_get_path('theme', 'bootstrap_business') .'/js/jquery.validate.js');    
    drupal_add_js(drupal_get_path('theme', 'bootstrap_business') .'/js/jqueryformvalidation.js');
    drupal_add_js(drupal_get_path('module', 'student') .'/js/custom_validations.js');
    drupal_add_css(drupal_get_path('theme', 'bootstrap_business') .'/css/jquery-ui.css');  
    drupal_add_js(drupal_get_path('theme', 'bootstrap_business') .'/js/jquery-ui.js');
    drupal_add_js('jQuery(document).ready(function($) { 
        $("#date_left").datepicker({
        changeMonth: true,
        changeYear: true,
        showOn: "both",
        yearRange: "1980:2015",
        buttonImage: "'.file_create_url('public://calendar.gif').'",
        buttonImageOnly: true,
        onSelect: function(selected) {
        }
        });
        
        $("#date_joined").datepicker({
        changeMonth: true,
        changeYear: true,
        yearRange: "1980:2015",
        showOn: "both",
        buttonImage: "'.file_create_url('public://calendar.gif').'",
        buttonImageOnly: true,
        onSelect: function(selected) {
        }
        });
        
        $("#graduationdate").datepicker({
        changeMonth: true,
        changeYear: true,
        showOn: "both",
        yearRange: "1980:2020",
        buttonImage: "'.file_create_url('public://calendar.gif').'",
        buttonImageOnly: true,
        onSelect: function(selected) {
        }
        });
        
        $("#graduation_date1").datepicker({
        changeMonth: true,
        changeYear: true,
        showOn: "both",
        yearRange: "1980:2020",
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
     
    
    $StdProgram = get_stdprogram($user->uid);     
    $KylyName = get_stdkulyname();
    $ProgramResults = get_stdresult();
    $ProgramCode = $StdProgram['program']; 
    $ProgramCodePrev = $StdProgram['prev_program'];
     
    $student_details = db_select('student_details')
        ->fields('student_details')    
         ->condition('uid', $user->uid, '=')
         ->execute();    
    while($row = $student_details->fetchAssoc($student_details)){
        $searchable = $row['searchable'];
        $experience_level = $row['experience_level'];
        $year = $row['working_year'];
        $month = $row['working_month'];
    }
    $specializationp = 0;
    $specializations = 0;
    $specializationc = 0;
    $country = 0;
    $location_country = array();
    $student_qualification = db_select('student_qualification')
        ->fields('student_qualification')    
         ->condition('uid', $user->uid, '=')
         ->execute();    
    while($row = $student_qualification->fetchAssoc($student_qualification)){
        $qualification[] = $row['qualification'];
        $qualification_level[] = $row['qualification_level'];
        $field_of_study[] = $row['field_of_study'];
        $university[] = $row['university']; 
        //$location[] = $row['location'];
        $location_country[] = $row['location_country'];
        $location_state[] = $row['location_state'];
        $graduation_date[] = date("m/d/Y", strtotime($row['graduation_date']));
        $highest[] = $row['highest']; 
        $major[] = $row['major']; 
        $grade[] = $row['grade']; 
        $additional_info[] = $row['additional_info']; 
    }
    
    $form = array();
    // To make these fields required
    
    
     $student_experience = db_select('student_experience')
        ->fields('student_experience')    
         ->condition('uid', $user->uid, '=')
         ->execute();    
    while($row = $student_experience->fetchAssoc($student_experience)){
        $company_name = $row['company_name'];
        $date1 = explode('-', $row['date_joined']);
        $date_joined = $date1[1].'/'.$date1[2].'/'.$date1[0];   
        if($row['date_left']!="") {
            $date2 = explode('-', $row['date_left']);
            $date_left = $date2[1].'/'.$date2[2].'/'.$date2[0]; 
        } else {
            $date_left = '';
        }
        $present = $row['present'];
        $job_title = $row['job_title'];
        $position = $row['position_level'];
        $country = $row['country'];
        $state = $row['state'];
        $salary = $row['salary'];
        $specializationp = $row['specialization_p']; 
        $specializations = $row['specialization_sp']; 
        $specializationc = $row['specialization_c']; 
        
        $description = $row['description']; 
        //$additional_info = $row['additional_info']; 
        $industry = $row['industry']; 
       // $type = $row['experience_level']; 
       // $experience_level = $row['experience_level']; 
    }
     
    $form['experience1'] = array(
        '#type' => 'fieldset', 
        '#title' => t(' '), 
    );  
    
    $form['experience1']['title'] = array(
        '#type' => 'item', 
        '#markup' => t(' <h2>Experience Level</h2>'), 
    ); 
     
    $form['experience1']['level'] = array(
            '#type' => 'radios',
            '#title' => t(''), 
            '#options' => array(1 => t('I am a fresh graduate seeking my first job'),
                                2 => t('I am a student seeking internship or part-time jobs'),
                                0 => t('I have been working since ') ),
           // '#required' => TRUE,
            '#default_value' => isset($experience_level) ? $experience_level : 2,
           
            '#prefix' => '<div>',
            '#suffix' => '</div>',
        );
    if($experience_level==0) {
        $style = 'style="display:block"';
    } else {
        $style = 'style="display:none"';
    }
    $form['experience1']['year'] = array(
        '#id' => 'year',
        '#type' => 'select',
        '#options' =>  array(''=>'Select') + getYear(),
        '#default_value' => $year,  
        '#prefix' => '<div id="WorkingDate" '.$style.'>',
    );
    
    $form['experience1']['month'] = array(
        '#id' => 'month',
        '#type' => 'select',
        '#options' =>  array(''=>'Select') + getMonth(),
        '#default_value' => $month,  
        '#suffix' => '</div>',
    );
    
     if(isset($experience_level) && $experience_level == 0)
        $display = 'style="display:block;"';
     else
         $display = 'style="display:none;"';
    $form['experience'] = array(
        '#type' => 'fieldset', 
        '#title' => t(' '), 
        '#prefix' => '<div id="ExperienceDiv" '.$display.'>',
        '#suffix' => '</div>',
    );  
    
     $form['experience']['title'] = array(
        '#type' => 'item', 
        '#markup' => '<h2>Work Experience</h2>', 
    );
     
    $form['experience']['company_name'] = array(
        '#type' => 'textfield',
        '#id' => t('company_name'),
        '#title' => t('Company Name <span class="form-required">*</span>'),
      //  '#required' => TRUE, // Added
        '#default_value' => (isset($company_name) ? $company_name : ''),  
    );
    
    /*$form['experience']['industry'] = array(
        '#type' => 'textfield',
        '#title' => t('University'),
        '#required' => TRUE, // Added
        '#default_value' => (isset($industry) ? $industry : ''),  
    );*/
    
    
    $form['experience']['industry'] = array(
        '#id' => t('industry'),
        '#type' => 'select',
        '#title' => t('Industry <span class="form-required">*</span>'),
        '#options' => array(''=>'Select')+nature_bussiness_arr(),
//        '#attributes' => array(
//            'name' => 'industry[]'
//        ),        
       // '#required' => TRUE,
        '#value' => (isset($industry) ? $industry : ''), 
    ); 

    $form['experience']['date_joined'] = array(
     '#type' => 'textfield', 
     '#id' => 'date_joined',
     '#title' => 'Date Joined <span class="form-required">*</span>',
        '#attributes' => array('readonly' => 'readonly'),
     //'#required' => TRUE, // Added
     '#default_value' => (isset($date_joined) ? $date_joined : ''),  
     );
      
    
     $form['experience']['date_left'] = array(
        '#title' => 'Date Left <span class="form-required">*</span>',  
        '#id' => 'date_left',
         '#attributes' => array('readonly' => 'readonly'),
        '#type' => 'textfield',
       // '#required' => TRUE,
        '#default_value' => (isset($date_left) ? $date_left : ''),  
        '#maxlength' => 14
    );
     $form['experience']['or'] = array(           
            '#title' => 'or',
            '#type' => 'item',
    );
      
      $form['experience']['present'] = array(
            '#id' => 'present',
            '#title' => 'Present (Currently Working)',
            '#type' => 'checkbox',
           // '#value' => 1,
            '#default_value' => (isset($present) ? $present : ''),  
        ); 
     
      $form['experience']['job_title'] = array(
        '#id' => 'job_title',
        '#title' => 'Job Title <span class="form-required">*</span>',        
        '#type' => 'textfield',
       // '#required' => TRUE,
        '#default_value' => (isset($job_title) ? $job_title : ''),  
        //'#maxlength' => 14
    );
     
     $form['experience']['position'] = array(
     '#id' => 'position',
     '#type' => 'select', 
     '#options' => array(''=>'Select') + positionoffered(),
     '#title' => 'Position Level <span class="form-required">*</span>',
     //'#required' => TRUE, // Added
     '#default_value' => (isset($position) ? $position : ''),
      
     );
         
     
     
     
     $category_parrents = db_select('specializations_p')
            ->fields('specializations_p')
            ->execute()
            ->fetchAll();
    $display_category_parrents[''] = 'All Specializations';
    foreach ($category_parrents as $category_parrent_name):
        $display_category_parrents[$category_parrent_name->sid] = $category_parrent_name->name;
    endforeach;
    
    $form['experience']['specializationp'] = array(
        '#id' => 'specializationp',
        '#type' => 'select',
        '#title' => t('Specialization <span class="form-required">*</span>'),
        '#options' => $display_category_parrents,
        '#ajax' => array(
            'event' => 'change',
            'callback' => 'ajax_specializations_callback',
            'wrapper' => 'subparrent-list-wrapper',
        ),
        '#default_value' => isset($specializationp)?$specializationp:'', 
    );
    
    // Wrapper for sub parent dropdown list
    $form['experience']['wrapper'] = array(
        '#prefix' => '<div id="subparrent-list-wrapper">',
        '#suffix' => '</div>',
    );
    
    if (isset($form_state['values']['specializationp']) && $form_state['values']['specializationp'] != "" || $specializationp>0) {
        $spid = isset($form_state['values']['specializationp'])?$form_state['values']['specializationp']:$specializationp;
        $category_subparrents = db_select('specializations_sp')
                ->fields('specializations_sp')
                ->condition('spid', $spid, '=')
                ->execute()
                ->fetchAll();
        $display_category_subparrents[''] = 'Select';
        foreach ($category_subparrents as $category_subparrent_name):
            $display_category_subparrents[$category_subparrent_name->sid] = $category_subparrent_name->name;
        endforeach;
        // Province dropdown list
        $form['experience']['specializations'] = array(
            '#id' => 'specializations',
            '#type' => 'select',
            '#options' => $display_category_subparrents,
            '#ajax' => array(
                'event' => 'change',
                'callback' => 'ajax_specializationc_callback',
                'wrapper' => 'child-list-wrapper',
            ),
            '#default_value' => $specializations, 
        );
    }
    
    // Wrapper for child dropdown list
    $form['experience']['wrapper_child'] = array(
        '#prefix' => '<div id="child-list-wrapper">',
        '#suffix' => '</div>',
    );
    
    if (isset($form_state['values']['specializations']) && $form_state['values']['specializations'] != "" || $specializations>0) {
        $cid = isset( $form_state['values']['specializations'])? $form_state['values']['specializations']:$specializations;
        $category_childs = db_select('specializations_c')
                ->fields('specializations_c')
                ->condition('cid', $cid, '=')
                ->execute()
                ->fetchAll();
        $display_category_childs[''] = 'Select';
        foreach ($category_childs as $category_child_name):
            $display_category_childs[$category_child_name->sid] = $category_child_name->name;
        endforeach;
        // Province dropdown list
        $form['experience']['specializationc'] = array(
            '#id' => 'specializationc',
            '#type' => 'select',
            '#options' => $display_category_childs,
            '#default_value' => $specializationc, 
        );
    }
      
  $form['experience']['location_country'] = array(
         '#id' => 'location_country',
        '#type' => 'select',
        '#title' => t('Work Location <span class="form-required">*</span>'),
        '#options' => array(''=>'Select') + countries_list(),
        '#ajax' => array(
            'event' => 'change',
            'callback' => 'student_location_state_list_ajax1',
            'wrapper' => 'loction-state-list-wrapper1',
        ),
        '#default_value' => (isset($country) ? $country : ''),
    );
    
    // Wrapper for state dropdown list
    $form['experience']['location']['wrapper'] = array(
        '#prefix' => '<div id="loction-state-list-wrapper1">',
        '#suffix' => '</div>',
    );
    
    if ((isset($form_state['values']['location_country']) && $form_state['values']['location_country'] != "") || $country>0) {
        $cid = isset($form_state['values']['location_country'])?$form_state['values']['location_country']:$country;
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
            $form['experience']['location']['wrapper']['location_state'] = array(
                '#id' => 'location_state',
                '#type' => 'select',
                '#title' => t('State <span class="form-required">*</span>'),
                '#multiple' => false,
                '#options' => $state_list_arr,
                '#default_value' => (isset($state) ? $state : ''),                
            );
        }
    }
 
        $form['experience']['salary'] = array(
           '#id' => 'salary',
           '#type' => 'textfield',
           '#title' => 'Monthly Salary (MYR) <span class="form-required">*</span>',
           '#description' => 'Enter Zero if there is no salary for this job',
           '#default_value' => (isset($salary) ? $salary: ''),  
        );
     
        $form['experience']['description'] = array(
           '#type' => 'text_format', 
           '#title' => 'Work Description',
          // '#required' => TRUE, // Added
           '#default_value' => (isset($description) ? $description : ''),  
        );     
     
        
        /*$form['experience']['additional_info'] = array(
           '#type' => 'textarea', 
           '#title' => 'Additional Info',
           '#required' => TRUE, // Added
           '#default_value' => (isset($additional_info) ? $additional_info : ''),  
        );*/
        
         $form['highest'] = array(
        '#type' => 'fieldset', 
        '#title' => t(' '), 
    );   
    
    $form['highest']['title'] = array(
        '#type' => 'item', 
        '#markup' => '<h2>Highest Qualification</h2>', 
    );  
    
    $output=$ProgramCode;                 
    $output.=$KylyName;
    
    $form['highest']['prgram'] = array(
        '#type' => 'item', 
        '#markup' => $output, 
    ); 
    
    if(!isset($qualification[0])) {
        $qualification[0] = 0;
    }
    
    if(!isset($qualification_level[0])) {
        $qualification_level[0] = 0;
    }
  
    $form['highest']['qualification'] = array(
    '#type' => 'select',
    '#id' => 'qualification',
    //'#required' => TRUE,
    '#title' => 'Highest Qualification <span class="form-required">*</span>',
    '#options' =>  array(''=>'Select') + qualification_list(),
    '#default_value' => (isset($qualification[0]) ? $qualification[0] : ''), 
    '#ajax' => array(
      // 'event' => 'change',
      'callback' => 'ajax_qualification_level_callback',
      'wrapper' => 'dropdown-highest-degree',
    ),
    
  );
    $selected = isset($form_state['values']['qualification']) ? $form_state['values']['qualification'] : $qualification[0];
    $form['highest']['qualification_level'] = array(
    '#type' => 'select',
    '#id' => 'qualificationlevel',
   // '#required' => TRUE,
    '#title' =>  t('Highest Qualification Level <span class="form-required">*</span>'),
    '#prefix' => '<div id="dropdown-highest-degree">',
    '#suffix' => '</div>',
    '#options' => array(''=>'Select') +highest_qualification_list($selected),
    '#default_value' => isset($form_state['values']['qualification_level']) ? $form_state['values']['qualification_level'] : $qualification_level[0],
  );  
    
    
    $form['highest']['field_of_study'] = array(
    '#id' => 'fieldofstudy',
    '#title' => 'Field of Study <span class="form-required">*</span>',       
    '#type' => 'select',
    '#options' => fields_of_study(),
   // '#required' => TRUE,
    '#default_value' => (isset($field_of_study[0]) ? $field_of_study[0] : ''),  
    '#maxlength' => 14,

    );
    
    
    
    
    
    /*$form['highest']['qualification'] = array(
        '#type' => 'textfield',
        '#title' => t('Highest Qualification'),
        '#required' => TRUE, // Added
        '#default_value' => (isset($qualification[0]) ? $qualification[0] : ''),  
    );*/
    /*$form['highest']['field_of_study'] = array(
        '#type' => 'textfield',
        '#title' => t('Field of Study'),
        '#required' => TRUE, // Added
        '#default_value' => (isset($field_of_study[0]) ? $field_of_study[0] : ''),  
    );*/
    
    $form['highest']['university'] = array(
        '#type' => 'textfield',
        '#id' => 'university',
        '#title' => t('Institute/University <span class="form-required">*</span>'),
        //'#required' => TRUE, // Added
        '#default_value' => (isset($university[0]) ? $university[0] : ''),  
    );
    
     $form['highest']['graduation_date'] = array(
        '#title' => 'Graduation Date <span class="form-required">*</span>',        
        '#id' => 'graduationdate',
         '#type' => 'textfield',
      //  '#required' => TRUE,
         '#attributes' => array('readonly' => 'readonly'),
        // '#description' => '(YYYY)',
        '#default_value' => (isset($graduation_date[0]) ? $graduation_date[0] : ''),  
        '#maxlength' => 14
    );
     
     $form['highest']['major'] = array(
        '#title' => 'Major',
        '#id' => 'major',
        '#type' => 'textfield',         
        '#default_value' => (isset($major[0]) ? $major[0] : ''),
     );
     
     $form['highest']['grade'] = array(
        '#title' => 'Grade',
        '#id' => 'grade',
        '#type' => 'select',
        '#options' => grade_list(),
        '#default_value' => (isset($grade[0]) ? $grade[0] : ''),
     );
    
     $form['highest']['additional_info'] = array(
        '#title' => 'Additional Information',
        '#id' => 'additional_info',
        '#type' => 'text_format',         
        '#default_value' => (isset($additional_info[0]) ? $additional_info[0] : ''),
     );
     
     
    $form['highest']['country'] = array(
         '#id' => 'country',
        '#type' => 'select',
        '#title' => t('Institute/University Location <span class="form-required">*</span>'),
        '#options' => array(''=>'Select') + countries_list(),
        '#ajax' => array(
            'event' => 'change',
            'callback' => 'student_upload_resume_state_list_ajax',
            'wrapper' => 'state-list-wrapper',
        ),
        '#default_value' => (isset($location_country[0]) ? $location_country[0] : ''),
    );
    
    // Wrapper for state dropdown list
    $form['highest']['wrapper'] = array(
        '#prefix' => '<div id="state-list-wrapper">',
        '#suffix' => '</div>',
    );
    
    if ((isset($form_state['values']['country']) && $form_state['values']['country'] != "") || (isset($location_country[0]) && $location_country[0]>0)) {        
        $cid = isset($form_state['values']['country'])?$form_state['values']['country']:$location_country[0];        
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
            $form['highest']['wrapper']['state'] = array(
                '#id' => 'state',
                '#type' => 'select',
                '#title' => t('State <span class="form-required">*</span>'),
                '#multiple' => false,
                '#options' => $state_list_arr,
                '#default_value' => (isset($location_state[0]) ? $location_state[0] : ''),                
            );
        }
    }  
    $form['result'] = array(
            '#type' => 'item',
            '#markup' => $ProgramResults
        );

    $form['qualification'] = array(
        '#id' => 'qualificationDiv',
        '#type' => 'fieldset', 
        '#title' => t('Show 2nd Highest Qualification'), 
        '#prefix' => '<div style="cursor: pointer">',
        '#suffix' => '</div>',
        ); 
    
    $form['second_highest'] = array(
        '#type' => 'fieldset', 
        '#title' => t(' '),
        '#prefix' => '<div id="secondHighestDiv" style="display:none">',
        '#suffix' => '</div>',
    ); 
     $form['second_highest']['title'] = array(
        '#type' => 'item', 
        '#markup' => '<h2>2nd Highest Qualification</h2>', 
    ); 
    
    $output=$ProgramCodePrev; 
    
     $form['second_highest']['program'] = array(
        '#type' => 'item', 
        '#markup' => $output, 
    ); 
     
     
     if(!isset($qualification[1])) {
        $qualification[1] = 1;
    }
    
    if(!isset($qualification_level[1])) {
        $qualification_level[1] = 1;
    }
    
    $form['second_highest']['qualification1'] = array(
    '#id' => 'qualification1',
    '#type' => 'select',
  //  '#required' => TRUE,
    '#title' => '2nd Highest Qualification <span class="form-required">*</span>',
    '#options' =>  array(''=>'Select') + qualification_list(),
    '#default_value' => '', 
    '#ajax' => array(
      // 'event' => 'change',
      'callback' => 'ajax_2ndqualification_level_callback',
      'wrapper' => 'dropdown-highest-degree1',
    ),
    
  );
    $selected = isset($form_state['values']['qualification1']) ? $form_state['values']['qualification1'] : $qualification[1];
    $form['second_highest']['qualification_level1'] = array(
    '#type' => 'select',
    '#id' => 'qualification_level1',
   // '#required' => TRUE,
    '#title' =>  t('Qualification Level <span class="form-required">*</span>'),
    '#prefix' => '<div id="dropdown-highest-degree1">',
    '#suffix' => '</div>',
    '#options' => array(''=>'Select') +highest_qualification_list($selected),
    '#default_value' => isset($form_state['values']['qualification_level1']) ? $form_state['values']['qualification_level1'] : $qualification_level[1],
  );  
    
    
    $form['second_highest']['field_of_study1'] = array(
     '#title' => 'Field of Study <span class="form-required">*</span>',   
     '#id' => 'field_of_study1',
    '#type' => 'select',
    '#options' => fields_of_study(),
   // '#required' => TRUE,
    '#default_value' => (isset($field_of_study[1]) ? $field_of_study[1] : ''),  
    '#maxlength' => 14,

    );
    
 
    
    $form['second_highest']['university1'] = array(
        '#type' => 'textfield',
        '#id' => 'university1',
        '#title' => t('Institute/University <span class="form-required">*</span>'),
      //  '#required' => TRUE, // Added
        '#default_value' => (isset($university[1]) ? $university[1] : ''),  
    );
      
    
     $form['second_highest']['graduation_date1'] = array(
        '#title' => 'Graduation Date <span class="form-required">*</span>',    
         '#id' => 'graduation_date1',
        '#type' => 'textfield',
         '#attributes' => array('readonly' => 'readonly'),
        // '#description' => '(YYYY)',
        //'#required' => TRUE,
        '#default_value' => (isset($graduation_date[1]) ? $graduation_date[1] : ''),  
        '#maxlength' => 14
    ); 
     
     $form['second_highest']['major1'] = array(
        '#title' => 'Major',
        '#id' => 'major',
        '#type' => 'textfield',         
        '#default_value' => (isset($major[1]) ? $major[1] : ''),
     );
     
     $form['second_highest']['grade1'] = array(
        '#title' => 'Grade',
        '#id' => 'grade',
        '#type' => 'select',
        '#options' => grade_list(),
        '#default_value' => (isset($grade[1]) ? $grade[1] : ''),
     );
    
     $form['second_highest']['additional_info1'] = array(
        '#title' => 'Additional Information',
        '#id' => 'additional_info',
        '#type' => 'text_format',         
        '#default_value' => (isset($additional_info[1]) ? $additional_info[1] : ''),
     );
     
     $form['second_highest']['country1'] = array(
         '#id' => 'country1',
        '#type' => 'select',
        '#title' => t('Institute/University Location <span class="form-required">*</span>'),
        '#options' => array(''=>'Select') + countries_list(),
        '#ajax' => array(
            'event' => 'change',
            'callback' => 'student_upload_state_list_ajax',
            'wrapper' => 'state-list-wrapper1',
        ),
        '#default_value' => (isset($location_country[1]) ? $location_country[1] : ''),
    );
    
    // Wrapper for state dropdown list
    $form['second_highest']['wrapper'] = array(
        '#prefix' => '<div id="state-list-wrapper1">',
        '#suffix' => '</div>',
    );
    
    if ((isset($form_state['values']['country1']) && $form_state['values']['country1'] != "") || (isset($location_country[1]) && $location_country[1] >0)) {
        $cid = isset($form_state['values']['country1'])?$form_state['values']['country1']:$location_country[1];        
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
            $form['second_highest']['wrapper']['state1'] = array(
                '#id' => 'state1',
                '#type' => 'select',
                '#title' => t('State <span class="form-required">*</span>'),
                '#multiple' => false,
                '#options' => $state_list_arr,
                '#default_value' => (isset($location_state[1]) ? $location_state[1] : ''),                
            );
        }
    }
    
     
     $form['second_highest']['qualification'] = array(
        '#id' => 'hideQualificationDiv',
        '#type' => 'fieldset', 
        '#title' => t('Hide 2nd Highest Qualificaiton'), 
        '#prefix' => '<div style="cursor: pointer">',
        '#suffix' => '</div>',
        ); 
        
        
      $form['privacytitle'] = array(
        '#type' => 'item', 
        '#markup' => '<h2>Privacy Setting</h2>', 
         );
        $form['searchable'] = array(
            '#id' => 'searchable',
            '#type' => 'radios',
             '#options' => array(
                1 => t('Searchable with Contact Details <span class="note_privacy">Allow employers to search for my resume and see my name and contact details.</span>'), 
                0 => t('Not Searchable <span class="note_privacy">Do not allow employers to search for my resume.</span>')
                ),
            //'#required' => TRUE,
            '#default_value' => (isset($searchable) ? $searchable : ''),
            
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

function student_upload_resume_form_submit($form, &$form_state) {
    global $user;
    extract($form_state['input']);
    //echo "<pre>";print_r($form_state['input']);
    if(isset($graduation_date)){
        $date = explode('/', $graduation_date);
        if(is_array($date))
            $graduation_date = $date[2].'-'.$date[0].'-'.$date[1];
    }
    
    if(isset($graduation_date1) && $graduation_date1!=''){
        $date = explode('/', $graduation_date1);
        if(is_array($date))
            $graduation_date1 = $date[2].'-'.$date[0].'-'.$date[1];
    }
    if(!isset($country1) || $country1=='')
        $country1 = 0;
    if(!isset($state1) || $state1=='')
        $state1 = 0;
    if(!isset($state) || $state=='')
        $state = 0;
    if(!isset($qualification_level) || $qualification_level=='')
        $qualification_level = 0;
    
    $detailsArr = array(
        'searchable' => $searchable,
        'experience_level' => $level
    );
    if($level==0){
        $detailsArr['working_year'] = $year;
        $detailsArr['working_month'] = $month;
    }
    db_update('student_details')
        ->fields($detailsArr)
        ->condition('uid', $user->uid, '=')
        ->execute();
     
    $student_qualification = db_select('student_qualification')
        ->fields('student_qualification')    
        ->condition('uid', $user->uid, '=')
        ->condition('highest', '1', '=')
        ->execute();  
    $count = $student_qualification->rowCount(); 
    if($count>0) {
        $eid = db_update('student_qualification')
        ->fields(array(
            'qualification' => $qualification,
            'qualification_level' => $qualification_level,
            'field_of_study' => $field_of_study,
	    'university' => $university,
            'location_country' => $country,
            'location_state' => $state,
            'graduation_date' => $graduation_date,
            'major' => $major,
            'grade' => $grade,
            'additional_info' => $additional_info['value'],
         ))
        ->condition('uid', $user->uid, '=')
        ->condition('highest', '1', '=')
        ->execute();
    } else {
        $eid = db_insert('student_qualification')
        ->fields(array(
            'qualification' => $qualification,
            'qualification_level' => $qualification_level,
            'field_of_study' => $field_of_study,
	    'university' => $university,           
            'location_country' => $country,
            'location_state' => $state,
            'graduation_date' => $graduation_date,
            'major' => $major,
                'grade' => $grade,
                'additional_info' => $additional_info['value'],
            'highest' => 1,
            'uid' => $user->uid,
            'created_date' => date('Y-m-d H:i:s')
        ))
        ->execute();
    }
    
    if(isset($qualification_level1) && $qualification_level1>0) {
        $student_qualification = db_select('student_qualification')
            ->fields('student_qualification')    
            ->condition('uid', $user->uid, '=')
            ->condition('highest', '2', '=')
            ->execute();  
        $count = $student_qualification->rowCount(); 
        if($count>0) {
            $eid = db_update('student_qualification')
            ->fields(array(
                'qualification' => $qualification1,
                'qualification_level' => $qualification_level1,
                'field_of_study' => $field_of_study1,
                'university' => $university1,
                'location_country' => $country1,
                'location_state' => $state1,
                'graduation_date' => $graduation_date1,
                'major' => $major1,
                'grade' => $grade1,
                'additional_info' => $additional_info1['value'],
             ))
            ->condition('uid', $user->uid, '=')
            ->condition('highest', '2', '=')
            ->execute();
        } else {
            $eid = db_insert('student_qualification')
            ->fields(array(
                'qualification' => $qualification1,
                'qualification_level' => $qualification_level1,
                'field_of_study' => $field_of_study1,
                'university' => $university1,
                'location_country' => $country1,
                'location_state' => $state1,
                'graduation_date' => $graduation_date1,
                'major' => $major1,
                'grade' => $grade1,
                'additional_info' => $additional_info1['value'],
                'highest' => 2,
                'uid' => $user->uid,
                'created_date' => date('Y-m-d H:i:s')
            ))
            ->execute();
        }
    }  
  if($level==0){  
      
    if(!isset($location_state) || $location_state=='')
        $location_state = 0;  
    $date1 = explode('/', $date_joined);
    $date_joined = $date1[2].'-'.$date1[0].'-'.$date1[1]; 
    
    //update experience
    $student_qualification = db_select('student_experience')
        ->fields('student_experience')    
        ->condition('uid', $user->uid, '=')
         ->execute();  
    $count = $student_qualification->rowCount();
    
    $params = array(
        'company_name' => $company_name,
        'industry' => $industry,
        'date_joined' => $date_joined,
        //'date_left' => $date_left,
        'job_title' => $job_title,
        'position_level' => $position,
        'specialization_p' => $specializationp,
        'specialization_sp' => $specializations,
        'specialization_c' => $specializationc,        
        'country' => $location_country,
        'state' => $location_state,
        'salary' => $salary,        
        'description' => $description['value'],
    );
    if($present){
        $date_left = '0000-00-00';
        $params['present'] = 1;
    } else {
        $date2 = explode('/', $date_left);
        $date_left = $date2[2].'-'.$date2[0].'-'.$date2[1];
        $present = 0;
        $params['present'] = 0;
        $params['date_left'] = $date_left;
    }
    
    if($count>0) {
        $eid = db_update('student_experience')
        ->fields($params)
        ->condition('uid', $user->uid, '=')
        ->execute();
    } else {
        $params['created_date'] = date('Y-m-d H:i:s');
        $params['uid'] = $user->uid;
        $eid = db_insert('student_experience')
        ->fields($params)
        ->execute();
    }
  }
  audit_log('insert', 'resume created.');
  if($level==0)
    drupal_goto('student/editexperience');
  else
    drupal_goto('student/editeducation');
}