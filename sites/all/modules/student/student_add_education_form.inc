<?php

function student_add_education_form($form, &$form_state) {
    global $user;
    global $base_url;
    drupal_add_js(drupal_get_path('theme', 'bootstrap_business') . '/js/jquery.validate.js');
    drupal_add_js(drupal_get_path('theme', 'bootstrap_business') . '/js/jqueryformvalidation.js');
    drupal_add_js(drupal_get_path('module', 'student') . '/js/custom_validations.js');
    drupal_add_css(drupal_get_path('theme', 'bootstrap_business') . '/css/jquery-ui.css');
    drupal_add_js(drupal_get_path('theme', 'bootstrap_business') . '/js/jquery-ui.js');
    drupal_add_js('jQuery(document).ready(function($) { 
        $("#date_left").datepicker({
        changeMonth: true,
        changeYear: true,
        showOn: "both",
        yearRange: "1980:2015",
        buttonImage: "' . file_create_url('public://calendar.gif') . '",
        buttonImageOnly: true,
        onSelect: function(selected) {
        }
        });
        
        $("#date_joined").datepicker({
        changeMonth: true,
        changeYear: true,
        showOn: "both",
        yearRange: "1980:2015",
        buttonImage: "' . file_create_url('public://calendar.gif') . '",
        buttonImageOnly: true,
        onSelect: function(selected) {
        }
        });
        
        $("#graduation_date").datepicker({
        changeMonth: true,
        changeYear: true,
        showOn: "both",
        yearRange: "1980:2020",
        buttonImage: "' . file_create_url('public://calendar.gif') . '",
        buttonImageOnly: true,
        onSelect: function(selected) {
        }
        });
        
    $("#graduation_date1").datepicker({
        changeMonth: true,
        changeYear: true,
        showOn: "both",
        yearRange: "1980:2020",
        buttonImage: "' . file_create_url('public://calendar.gif') . '",
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
    $KulyName = get_stdkulyname();
    $ProgramResult = get_stdresult();


    $student_qualification = db_select('student_qualification')
            ->fields('student_qualification')
            ->condition('uid', $user->uid, '=')
            ->execute();
    $count = $student_qualification->rowCount();
    $output = '';

    $output .= ' 
         <h2>Education <span> - Please add your two highest education qualifications</span></h2>';
    if ($count == 0) {
        $programCode = $StdProgram['program'];
    } else {
        $programCode = $StdProgram['prev_program'];
    }


    $form = array();
    // To make these fields required 

    $form['content'] = array(
        '#type' => 'item',
        '#markup' => $output,
    );

    $form['highest'] = array(
        '#type' => 'fieldset',
        '#prefix' => '<div id="HighestDiv1" style="display:block">',
        '#suffix' => '</div>',
    );
    if ($count == 0) {
        $form['highest']['title'] = array(
            '#type' => 'item',
            '#markup' => '<h2>Highest Qualification</h2>',
        );
        $form['highest']['programCode'] = array(
            '#type' => 'item',
            '#title' => 'Program info from existing database',
            '#markup' => $programCode,
        );
    } else {
        $form['highest']['title'] = array(
            '#type' => 'item',
            '#markup' => '<h2>2nd Highest Qualification</h2>',
        );
        $form['highest']['programCode'] = array(
            '#type' => 'item',
            '#title' => 'Program info from existing database',
            '#markup' => $programCode,
        );
    }
    if (!isset($qualification[0])) {
        $qualification[0] = 0;
    }

    if (!isset($qualification_level[0])) {
        $qualification_level[0] = 0;
    }

    $form['highest']['qualification'] = array(
        '#id' => 'qualification',
        '#type' => 'select',
        // '#required' => TRUE,
        '#title' => 'Highest Qualification <span class="form-required">*</span>',
        '#options' => array('' => 'Select') + qualification_list(),
        // '#default_value' => (isset($qualification[0]) ? $qualification[0] : ''), 
        '#ajax' => array(
            // 'event' => 'change',
            'callback' => 'ajax_qualification_level_callback',
            'wrapper' => 'dropdown-highest-degree',
        ),
    );
    $selected = isset($form_state['values']['qualification']) ? $form_state['values']['qualification'] : $qualification[0];
    $form['highest']['qualification_level'] = array(
        '#id' => 'qualification_level',
        '#type' => 'select',
        //'#required' => TRUE,
        '#title' => t('Highest Qualification Level <span class="form-required">*</span>'),
        '#prefix' => '<div id="dropdown-highest-degree">',
        '#suffix' => '</div>',
        '#options' => array('' => 'Select') + highest_qualification_list($selected),
            // '#default_value' => isset($form_state['values']['qualification_level']) ? $form_state['values']['qualification_level'] : $qualification_level[0],
    );


    $form['highest']['field_of_study'] = array(
        '#id' => 'field_of_study',
        '#title' => 'Field of Study <span class="form-required">*</span>',
        '#type' => 'select',
        '#options' => array('' => 'Select') + fields_of_study(),
        // '#required' => TRUE,
        // '#default_value' => (isset($field_of_study[0]) ? $field_of_study[0] : ''),  
        '#maxlength' => 14,
    );



    $form['highest']['university'] = array(
        '#id' => 'university',
        '#type' => 'textfield',
        '#title' => t('Institute/University <span class="form-required">*</span>'),
            // '#required' => TRUE, // Added
            //   '#default_value' => (isset($university[0]) ? $university[0] : ''),  
    );




    $form['highest']['graduation_date'] = array(
        '#title' => 'Graduation Date <span class="form-required">*</span>',
        '#id' => 'graduation_date',
        '#type' => 'textfield',
        '#attributes' => array('readonly' => 'readonly'),
        '#description' => 'If still studying, enter expected graduation date',
        //  '#default_value' => (isset($graduation_date[0]) ? $graduation_date[0] : ''),  
        '#maxlength' => 14
    );
    $form['highest']['country'] = array(
        '#id' => 'country',
        '#type' => 'select',
        '#title' => t('Institute/University Location <span class="form-required">*</span>'),
        '#options' => array('' => 'Select') + countries_list(),
        '#ajax' => array(
            'event' => 'change',
            'callback' => 'student_upload_state_list_ajax1',
            'wrapper' => 'state-list-wrapper',
        ),
            // '#default_value' => (isset($location_country[0]) ? $location_country[0] : ''),
    );

    // Wrapper for state dropdown list
    $form['highest']['address']['wrapper'] = array(
        '#prefix' => '<div id="state-list-wrapper">',
        '#suffix' => '</div>',
    );

    if ((isset($form_state['values']['country']) && $form_state['values']['country'] != "")) {
        $cid = $form_state['values']['country'];
        $state_list = db_select('states_list')
                ->fields('states_list')
                ->condition('cid', $cid, '=')
                ->execute()
                ->fetchAll();
        $state_list_arr[''] = 'Select';
        foreach ($state_list as $value):
            $state_list_arr[$value->id] = $value->name;
        endforeach;
        if (count($state_list) > 0) {
            // Province dropdown list
            $form['highest']['address']['wrapper']['state'] = array(
                '#id' => 'state',
                '#type' => 'select',
                '#title' => t('State <span class="form-required">*</span>'),
                '#multiple' => false,
                '#options' => $state_list_arr,
                    //'#default_value' => (isset($location_state[0]) ? $location_state[0] : ''),                
            );
        }
    }

    $form['highest']['major'] = array(
        '#title' => 'Major',
        '#id' => 'major',
        '#type' => 'textfield',
            //'#default_value' => (isset($major[0]) ? $major[0] : ''),
    );

    $form['highest']['grade'] = array(
        '#title' => 'Grade',
        '#id' => 'grade',
        '#type' => 'select',
        '#options' => grade_list(),
            //'#default_value' => (isset($grade[0]) ? $grade[0] : ''),
    );
    
    if ($count == 0) {
        $form['highest']['kulyname'] = array(
            '#type' => 'item',
            '#title' => 'Kulliyyah information from existing database',
            '#markup' => $KulyName,
        );
        $form['highest']['kulyresults'] = array(
            '#type' => 'item',
            '#title' => 'Results from existing database',
            '#markup' => $ProgramResult,
        );
    }
    
    $form['highest']['additional_info'] = array(
        '#title' => 'Additional Information',
        '#id' => 'additional_info',
        '#type' => 'text_format',
            //'#default_value' => (isset($additional_info[0]) ? $additional_info[0] : ''),
    );

    $form['buttonsField'] = array(
        '#type' => 'fieldset',
        '#title' => t(''),
        '#prefix' => '<div id="buttonsDiv" style="display:block">',
        '#suffix' => '</div>',
    );
    $form['buttonsField']['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Submit'),
        '#prefix' => '<div class="submit_clear_but">',
    );
    $form['buttonsField']['cancel'] = array(
        '#type' => 'item',
        '#markup' => t('<a  class="experience_calcel" href="' . $base_url . '/student/editeducation">Cancel</a>'),
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
            <a class="active" id="nav_education" href="' . $base_url . '/student/editeducation">
            <div class="education borderRound" >
            <i rel="icon-education" class="icon-education" id="icon-education"></i></div><div id="nav-lbl-education">Education</div></a>
             
        </li>
        <li>
            <a id="nav_skills" href="' . $base_url . '/student/editskills">
            <div class="skills borderRound" >
            <i rel="icon-compass" class="icon-compass" id="icon-compass"></i></div><div id="nav-lbl-skill">Skills</div></a>
             
        </li>
        <li>
            <a id="nav_languages" href="' . $base_url . '/student/editlanguages">
            <div class="languages borderRound" >
            <i rel="icon-chat" class="icon-chat" id="icon-chat"></i></div><div id="nav-lbl-language">Languages</div></a>
            
        </li>
        <li>
            <a id="nav_add_info" href="' . $base_url . '/student/editadditionalinfo">
            <div class="additional_info borderRound" >
            <i rel="icon-additional" class="icon-additional" id="icon-additional"></i></div><div id="nav-lbl-additional">Additional Info</div></a>
             
        </li>
        <li>
            <a id="nav_about_me" href="' . $base_url . '/student/aboutme">
            <div class="about_me borderRound" >
            <i rel="icon-user" class="icon-user" id="icon-user"></i></div><div id="nav-lbl-about">About Me</div></a>
             
        </li>
        <li>
            <a id="previewResumeNav" href="' . $base_url . '/student/resumepreview">
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

function student_add_education_form_submit($form, &$form_state) {
    global $user;
    extract($form_state['input']);
    if (isset($graduation_date)) {
        $date = explode('/', $graduation_date);
        if (is_array($date))
            $graduation_date = $date[2] . '-' . $date[0] . '-' . $date[1];
    }

    if (!isset($state) || $state == '')
        $state = 0;
    if (!isset($field_of_study1) || $field_of_study1 == '')
        $field_of_study1 = 0;

    if (isset($graduation_date1) && $graduation_date1 != '') {
        $date = explode('/', $graduation_date1);
        if (is_array($date))
            $graduation_date1 = $date[2] . '-' . $date[0] . '-' . $date[1];
    }
    $student_qualification = db_select('student_qualification')
            ->fields('student_qualification')
            ->condition('uid', $user->uid, '=')
            ->execute();
    $count = $student_qualification->rowCount();
    if ($count == 0) {
        $highest = 1;
    } else {
        $highest = 2;
    }
    if ($count < 2) {
        $eid = db_insert('student_qualification')
                ->fields(array(
                    'qualification' => $qualification,
                    'qualification_level' => $qualification_level,
                    'field_of_study' => $field_of_study,
                    'university' => $university,
                    //'location' => $location,
                    'location_country' => $country,
                    'location_state' => $state,
                    'graduation_date' => $graduation_date,
                    'highest' => $highest,
                    'major' => $major,
                    'grade' => $grade,
                    'additional_info' => $additional_info['value'],
                    'uid' => $user->uid,
                    'created_date' => date('Y-m-d H:i:s')
                ))
                ->execute();
        audit_log('insert', 'Education Added in manage resume.');
    }
    drupal_set_message('Qualification Added');
    drupal_goto('student/editeducation');
}