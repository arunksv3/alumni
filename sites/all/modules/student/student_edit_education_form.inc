<?php

function student_edit_education_form($form, &$form_state) {
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
    $KylyName = get_stdkulyname();
    $ProgramResults = get_stdresult();
    $ProgramCode = $StdProgram['program'];
    $ProgramCodePrev = $StdProgram['prev_program'];

    $location_country = array();
    $location_state = array();
    $student_qualification = db_select('student_qualification')
            ->fields('student_qualification')
            ->condition('uid', $user->uid, '=')
            ->orderBy('id', 'desc')
            ->execute();
    $count = $student_qualification->rowCount();
    $output = '';
    $i = 0;
    while ($row = $student_qualification->fetchAssoc($student_qualification)) {
        $id[] = $row['id'];
        $qualification_name[] = get_qualiafication_name($row['qualification']);
        $qualification_level_name[] = get_qualiafication_level_name($row['qualification_level']);
        $field_of_study_name[] = fields_of_study_name($row['field_of_study']);
        $qualification[] = $row['qualification'];
        $qualification_level[] = $row['qualification_level'];
        $field_of_study[] = $row['field_of_study'];
        $university[] = $row['university'];
        $location_country[] = $row['location_country'];
        $location_state[] = $row['location_state'];
        $graduation_date[] = date('m/d/Y', strtotime($row['graduation_date']));
        $graduation[] = date('Y', strtotime($row['graduation_date']));
        $highest[] = $row['highest'];
        $major[] = $row['major'];
        $grade[] = $row['grade'];
        $additional_info[] = $row['additional_info'];
    }
    $gradeArr = grade_list();
    $output .= ' 
         <h2>Education <span> - Please add your two highest education qualifications</span></h2>';

    if ($count < 2) {
        $output.='<div id="addEducation" class="row" > 
                <div  class="col-md-10 resume_left">&nbsp;</div>
                <div class="col-md-2 resume_right addeducation">
                <a href="' . $base_url . '/student/addeducation">Add</a>
                 </div></div>';
    }
    if ($count >= 1) {

        for ($i = 0; $i < $count; $i++) {
            $n = $i + 1;
            $output .= '      <div id="Education' . $n . '" class="row" >               
                <div class="col-md-2 resume_left">
                ' . $graduation[$i] . '
                </div>
                <div class="col-md-8 resume_mid">
                    <h1>' . $university[$i] . '</h1>
                    <h2>' . $qualification_name[$i] . ' - ' . $qualification_level_name[$i] . '</h2>
                    <h2> in ' . $field_of_study_name[$i] . ' | ' . get_country_name($location_country[$i]) . ' ' . get_state_name($location_state[$i]) . '</h2>
                <ul> ';
            if ($major[$i] != '') {
                $output .= ' <li><label>Major: </label><span>' . $major[$i] . '</span></li> ';
            }
            if ($grade[$i] > 0) {
                $output .= '<li><label>Grade: </label><span>' . $gradeArr[$grade[$i]] . '</span></li>';
            }
            if ($additional_info[$i] != '') {
                $output .= ' <li><label>&nbsp;</label><span>' . $additional_info[$i] . '</span></li>';
            }
            $output .= ' </ul>                
                </div>
                <div class="col-md-2 resume_right">
                <a id="editEducationDiv' . $n . '" style="cursor: pointer;"> <i id="icon-edit" class="icon-edit"></i></a>&nbsp;&nbsp;<a  onclick="return confirm(\'Delete Education?\')" href="' . $base_url . '/student/deleteeducation/' . encrypt($id[$i], variable_get('IIUM_Encryption_Key', NULL)) . '/delete"><i id="icon-delete" class="icon-delete"></i></a></div>
                </div>
            ';

            if ($i == 0) {
                $output.=$ProgramCode;
                $output.=$KylyName;
                $output.=$ProgramResults;
            } else {
                $output.=$ProgramCodePrev;
            }
        }

        $output.='</div>';
    }
    /* if($count>1){     
      $output .= '     <div id="Education2" class="row" >
      <div class="col-md-2 resume_left">
      '.$graduation[1].'
      </div>
      <div class="col-md-8 resume_mid">

      <h1>'.$university[1].'</h1>
      <h2>'.$qualification_name[1].' - '.$qualification_level_name[1].'</h2>
      <h2> in '.$field_of_study_name[1].' | '.get_country_name($location_country[1]).' '.get_state_name($location_state[1]).'</h2>'
      . '<ul>';
      if($major[1]!='') {
      $output .= '<li><label>Major: </label><span>'.$major[1].'</span></li> ';
      }
      if($grade[1]>0) {
      $output .= '<li><label>Grade: </label><span>'.$gradeArr[$grade[1]].'</span></li>';
      }
      if($additional_info[1]!='') {
      $output .= '<li><label>&nbsp;</label><span>'.$additional_info[1].'</span></li>';
      }
      $output .= '</ul>
      </div>
      <div class="col-md-2 resume_right">
      <a id="editEducationDiv2" style="cursor: pointer;"> <i id="icon-edit" class="icon-edit"></i></a>&nbsp;&nbsp;<a onclick="return confirm(\'Delete Education?\')" href="'.$base_url.'/student/deleteeducation/'.encrypt($id[1],variable_get('IIUM_Encryption_Key', NULL)).'/delete"><i id="icon-delete" class="icon-delete"></i></a>
      </div>
      </div>
      ';
      } */
    $form = array();
    // To make these fields required 

    $form['content'] = array(
        '#type' => 'item',
        '#markup' => $output,
    );

    $form['highest'] = array(
        '#type' => 'fieldset',
        '#prefix' => '<div id="HighestDiv1" style="display:none">',
        '#suffix' => '</div>',
    );

    $form['highest']['title'] = array(
        '#type' => 'item',
        '#markup' => '<h2>Highest Qualification</h2>',
    );

    $form['highest']['program_code'] = array(
        '#type' => 'item',
        '#markup' => $ProgramCode,
    );

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
        '#default_value' => (isset($qualification[0]) ? $qualification[0] : ''),
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
        '#default_value' => isset($form_state['values']['qualification_level']) ? $form_state['values']['qualification_level'] : $qualification_level[0],
    );


    $form['highest']['field_of_study'] = array(
        '#id' => 'field_of_study',
        '#title' => 'Field of Study <span class="form-required">*</span>',
        '#type' => 'select',
        '#options' => array('' => 'Select') + fields_of_study(),
        // '#required' => TRUE,
        '#default_value' => (isset($field_of_study[0]) ? $field_of_study[0] : ''),
        '#maxlength' => 14,
    );



    $form['highest']['university'] = array(
        '#id' => 'university',
        '#type' => 'textfield',
        '#title' => t('Institute/University <span class="form-required">*</span>'),
        // '#required' => TRUE, // Added
        '#default_value' => (isset($university[0]) ? $university[0] : ''),
    );

    $form['edu_type'] = array(
        '#type' => 'hidden'
    );


    /*
      $form['highest']['location'] = array(
      '#id' => 'location',
      '#type' => 'textfield',
      '#title' => 'location',
      '#required' => TRUE, // Added
      '#default_value' => (isset($location[0]) ? $location[0] : ''),
      ); */


    $form['highest']['graduation_date'] = array(
        '#title' => 'Graduation Date <span class="form-required">*</span>',
        '#id' => 'graduation_date',
        '#type' => 'textfield',
        '#attributes' => array('readonly' => 'readonly'),
        '#description' => 'If still studying, enter expected graduation date',
        '#default_value' => (isset($graduation_date[0]) ? $graduation_date[0] : ''),
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
        '#default_value' => (isset($location_country[0]) ? $location_country[0] : ''),
    );

    // Wrapper for state dropdown list
    $form['highest']['address']['wrapper'] = array(
        '#prefix' => '<div id="state-list-wrapper">',
        '#suffix' => '</div>',
    );
    if (!isset($location_country[0])) {
        $location_country[0] = 0;
    }
    if ((isset($form_state['values']['country']) && $form_state['values']['country'] != "") || $location_country[0] > 0) {
        $cid = isset($form_state['values']['country']) ? $form_state['values']['country'] : $location_country[0];
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
                '#default_value' => (isset($location_state[0]) ? $location_state[0] : ''),
            );
        }
    }
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
    $form['highest']['kuly_name'] = array(
        '#type' => 'item',
        '#markup' => $KylyName,
    );
    $form['highest']['program_results'] = array(
        '#type' => 'item',
        '#markup' => $ProgramResults,
    );
    $form['highest']['additional_info'] = array(
        '#title' => 'Additional Information',
        '#id' => 'additional_info',
        '#type' => 'text_format',
        '#default_value' => (isset($additional_info[0]) ? $additional_info[0] : ''),
    );
    $form['second_highest'] = array(
        '#type' => 'fieldset',
        // '#title' => t('2nd Highest Qualification'),
        '#prefix' => '<div id="HighestDiv2" style="display:none">',
        '#suffix' => '</div>',
    );

    $form['second_highest']['title'] = array(
        '#type' => 'item',
        '#markup' => '<h2>2nd Highest Qualification</h2>',
    );

    $form['second_highest']['program_code'] = array(
        '#type' => 'item',
        '#markup' => $ProgramCodePrev,
    );
    if (!isset($qualification[1])) {
        $qualification[1] = 1;
    }

    if (!isset($qualification_level[1])) {
        $qualification_level[1] = 1;
    }

    $form['second_highest']['qualification1'] = array(
        '#id' => 'qualification1',
        '#type' => 'select',
        //  '#required' => TRUE,
        '#title' => '2nd Highest Qualification',
        '#options' => array('' => 'Select') + qualification_list(),
        '#default_value' => (isset($qualification[1]) ? $qualification[1] : ''),
        '#ajax' => array(
            // 'event' => 'change',
            'callback' => 'ajax_2ndqualification_level_callback',
            'wrapper' => 'dropdown-highest-degree1',
        ),
    );
    $selected = isset($form_state['values']['qualification1']) ? $form_state['values']['qualification1'] : $qualification[1];
    $form['second_highest']['qualification_level1'] = array(
        '#id' => 'qualification_level1',
        '#type' => 'select',
        // '#required' => TRUE,
        '#title' => t('Qualification Level'),
        '#prefix' => '<div id="dropdown-highest-degree1">',
        '#suffix' => '</div>',
        '#options' => array('' => 'Select') + highest_qualification_list($selected),
        '#default_value' => isset($form_state['values']['qualification_level1']) ? $form_state['values']['qualification_level1'] : $qualification_level[1],
    );


    $form['second_highest']['field_of_study1'] = array(
        '#id' => 'field_of_study1',
        '#title' => 'Field of Study',
        '#type' => 'select',
        '#options' => array('' => 'Select') + fields_of_study(),
        // '#required' => TRUE,
        '#default_value' => (isset($field_of_study[1]) ? $field_of_study[1] : ''),
        '#maxlength' => 14,
    );
    /*
      // To make these fields required
      $form['second_highest']['qualification1'] = array(
      '#type' => 'textfield',
      '#title' => t('2nd Highest Qualification'),
      '#required' => TRUE, // Added
      '#default_value' => (isset($qualification[1]) ? $qualification[1] : ''),
      );
      $form['second_highest']['field_of_study1'] = array(
      '#type' => 'textfield',
      '#title' => t('Field of Study'),
      '#required' => TRUE, // Added
      '#default_value' => (isset($field_of_study[1]) ? $field_of_study[1] : ''),
      ); */

    $form['second_highest']['university1'] = array(
        '#id' => 'university1',
        '#type' => 'textfield',
        '#title' => t('University'),
        //  '#required' => TRUE, // Added
        '#default_value' => (isset($university[1]) ? $university[1] : ''),
    );


    /* $form['second_highest']['location1'] = array(
      '#id' => 'location1',
      '#type' => 'textfield',
      '#title' => 'location',
      //'#required' => TRUE, // Added
      '#default_value' => (isset($location[1]) ? $location[1] : ''),
      ); */


    $form['second_highest']['graduation_date1'] = array(
        '#title' => 'Graduation Date',
        '#id' => 'graduation_date1',
        '#type' => 'textfield',
        '#attributes' => array('readonly' => 'readonly'),
        '#description' => 'If still studying, enter expected graduation date',
        '#default_value' => (isset($graduation_date[1]) ? $graduation_date[1] : ''),
        '#maxlength' => 14
    );


    $form['second_highest']['country1'] = array(
        '#id' => 'country1',
        '#type' => 'select',
        '#title' => t('Country'),
        '#options' => array('' => 'Select') + countries_list(),
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

    if ((isset($form_state['values']['country1']) && $form_state['values']['country1'] != "") || (isset($location_country[1]) && $location_country[1] > 0)) {
        $cid = isset($form_state['values']['country1']) ? $form_state['values']['country1'] : $location_country[1];
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
            $form['second_highest']['wrapper']['state1'] = array(
                '#id' => 'state1',
                '#type' => 'select',
                '#title' => t('State'),
                '#multiple' => false,
                '#options' => $state_list_arr,
                '#default_value' => (isset($location_state[1]) ? $location_state[1] : ''),
            );
        }
    }
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

    $form['buttonsField'] = array(
        '#type' => 'fieldset',
        '#title' => t(''),
        '#prefix' => '<div id="buttonsDiv" style="display:none">',
        '#suffix' => '</div>',
    );
    $form['buttonsField']['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Submit'),
        '#prefix' => '<div class="submit_clear_but">',
    );
    $form['buttonsField']['cancel_aboutme'] = array(
        '#type' => 'item',
        '#markup' => t('<a  class="experience_calcel" href="' . $base_url . '/student/editeducation">Cancel</a>'),
        '#suffix' => '</div>',
    );
    /* $form['buttonsField']['cancel'] = array(
      '#type' => 'fieldset',
      '#value' => t('Cancel'),
      '#prefix' => '<div id="hideEducationDiv1" style="cursor:pointer;background-color:#663398;width:100px;padding:10px;color:#FFF">',
      '#suffix' => '</div></div>',
      ); */

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

function student_edit_education_form_submit($form, &$form_state) {
    global $user;
    extract($form_state['input']);
    if (isset($graduation_date)) {
        $date = explode('/', $graduation_date);
        if (is_array($date))
            $graduation_date = $date[2] . '-' . $date[0] . '-' . $date[1];
    }
    if (!isset($state) || $state == '')
        $state = 0;
    if (!isset($state1) || $state1 == '')
        $state1 = 0;

    if (isset($graduation_date1) && $graduation_date1 != '') {
        $date = explode('/', $graduation_date1);
        if (is_array($date))
            $graduation_date1 = $date[2] . '-' . $date[0] . '-' . $date[1];
    }
    $student_qualification = db_select('student_qualification')
            ->fields('student_qualification')
            ->condition('uid', $user->uid, '=')
            //->condition('highest', '2', '=')
            ->execute();
    $count = $student_qualification->rowCount();

    if ($count > 0) {
        if ($edu_type == '1') {
            $query = db_update('student_qualification');
            $query->fields(array(
                'qualification' => $qualification1,
                'qualification_level' => $qualification_level1,
                'field_of_study' => $field_of_study1,
                'university' => $university1,
                //'location' => $location1,
                'location_country' => $country1,
                'location_state' => $state1,
                'graduation_date' => $graduation_date1,
                'major' => $major1,
                'grade' => $grade1,
                'additional_info' => $additional_info1['value'],
            ));
            $query->condition('uid', $user->uid, '=');
            if ($count == 2)
                $query->condition('highest', '1', '=');
            $query->execute();
            audit_log('update', 'Education updated in manage resume.');
        } else {
            $query = db_update('student_qualification');
            $query->fields(array(
                'qualification' => $qualification,
                'qualification_level' => $qualification_level,
                'field_of_study' => $field_of_study,
                'university' => $university,
                //'location' => $location,
                'location_country' => $country,
                'location_state' => $state,
                'graduation_date' => $graduation_date,
                'major' => $major,
                'grade' => $grade,
                'additional_info' => $additional_info['value'],
            ));
            $query->condition('uid', $user->uid, '=');
            if ($count == 2)
                $query->condition('highest', '2', '=');
            $query->execute();
            audit_log('update', 'Education updated in manage resume.');
        }
    } else {
        if ($edu_type == '1') {
            $eid = db_insert('student_qualification')
                    ->fields(array(
                        'qualification' => $qualification1,
                        'qualification_level' => $qualification_level1,
                        'field_of_study' => $field_of_study1,
                        'university' => $university1,
                        //'location' => $location1,
                        'location_country' => $country1,
                        'location_state' => $state1,
                        'graduation_date' => $graduation_date1,
                        'major' => $major1,
                        'grade' => $grade1,
                        'additional_info' => $additional_info1['value'],
                        'highest' => '1',
                        'uid' => $user->uid,
                        'created_date' => date('Y-m-d H:i:s')
                    ))
                    ->execute();
            audit_log('insert', 'Educatoin added in manage resume.');
        } else {
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
                        'major' => $major,
                        'grade' => $grade,
                        'additional_info' => $additional_info['value'],
                        'highest' => '2',
                        'uid' => $user->uid,
                        'created_date' => date('Y-m-d H:i:s')
                    ))
                    ->execute();
            audit_log('insert', 'Education added in manage resume.');
        }
    }
}
