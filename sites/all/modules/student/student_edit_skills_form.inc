<?php

function student_edit_skills_form($form, &$form_state) {
    global $user;
    global $base_url;
    drupal_add_js(drupal_get_path('theme', 'bootstrap_business') . '/js/jquery.validate.js');
    drupal_add_js(drupal_get_path('theme', 'bootstrap_business') . '/js/jqueryformvalidation.js');
    drupal_add_js(drupal_get_path('module', 'student') . '/js/custom_validations.js');
    drupal_add_css(drupal_get_path('theme', 'bootstrap_business') . '/css/jquery-ui.css');
    drupal_add_js(drupal_get_path('theme', 'bootstrap_business') . '/js/jquery-ui.js');



    $student_additional_info = db_select('student_skills')
            ->fields('student_skills')
            ->condition('uid', $user->uid, '=')
            ->execute();
    $i = 2;
    $form = array(); 
    
    $count = $student_additional_info->rowCount(); 
   
     $form['edit_skills'] = array(
        '#type' => 'fieldset',
        '#prefix' => '<div id="SkillsDiv" style="display:block">',
        '#suffix' => '</div>',
    );
   /* $form['edit_skills_link'] = array(
        '#type' => 'fieldset',
        '#title' => t('Edit'),
        '#prefix' => '<div id="EditSkills" style="cursor: pointer;float:right;width:100px;font-weight:bold;">',
        '#suffix' => '</div>',
    );*/
    $form['edit_skills']['title'] = array(
        '#type' => 'item',
        '#markup' => '<h2>Skills</h2>',         
    );
   
   $output = '';
   $output .= ' <li><div id="EditSkills" class="add-but"> Add </div></li>'; 
    if($count>0) {
   $output .= ' <table border="0">
                <tr><th>Skill</th><th>Proficiency</th><th>Delete</th></tr>';
            while ($row = $student_additional_info->fetchAssoc($student_additional_info)) {
        $id = $row['id'];
        $skills = $row['skills'];
        if ($row['proficiency'] == '1')
            $proficiency = 'beginner';
        else if ($row['proficiency'] == '2')
            $proficiency = 'Intermediate';
        else
            $proficiency = 'Advanced';    
               $output .= '      <tr><td>'.$skills.'</td><td>'.$proficiency.'</td>
                    <td><a  onclick="return confirm(\'Delete Skills?\')" href="'.$base_url.'/student/deleteskills/'.encrypt($id,variable_get('IIUM_Encryption_Key', NULL)).'/delete"><i id="icon-delete" class="icon-delete"></i></a></td></tr> ';
           
            } 
             $output .= ' </table>
            '; 
    }
               $output .= ' 
            </li> '; 
     
    
 $form['edit_skills']['skills'] = array(
            '#type' => 'item',
           // '#title' => t(isset($proficiency) ? $proficiency . ':' : '' . ':'),
            '#markup' => '
                <div id="Education1" class="row" >  
                
                <div class="col-md-12 resume_mid">
                <ul>'.$output.'</ul>
                         
                </div>
               
                </div>
                ',
        );
    
      $form['edit'] = array(
        '#type' => 'fieldset',
        
        '#prefix' => '<div id="EditSkillsDiv" style="display:none">',
       // '#suffix' => '</div>',
    );  
 
 

    // Because we have many fields with the same values, we have to set
    // #tree to be able to access them.
    $form['edit_skills1']['#tree'] = TRUE;
    $form['edit_skills1'] = array(
        '#type' => 'fieldset',
       // '#title' => t('Edit Skills'),
        // Set up the wrapper so that AJAX will be able to replace the fieldset.
        '#prefix' => ' '
        . '<div id="names-fieldset-wrapper">',
        '#suffix' => ' </div>',
    );
    $form['edit_skills1']['title'] = array(
        '#type' => 'item',
        '#markup' => '<h2>Add Skills</h2>',         
    );
    // Build the fieldset with the proper number of names. We'll use
    // $form_state['num_names'] to determine the number of textfields to build.
    if (empty($form_state['num_names'])) {
        $form_state['num_names'] = 1;
    }

    for ($i = 0; $i < $form_state['num_names']; $i++) {

        //lets add all the fields we want in the set  
        /*
          We have the prefix and suffix added here, so that we can do some sort of styling with the form, like display the fields side by side. You may remove    it, but generally we need that when we have a set of fields, hence I thought to keep it here.
         */

        //Parent container
        $form['edit_skills1'][$i] = array(
            '#prefix' => '<div class="two-col">',
            '#suffix' => '</div>'
        );

        //other form elements
        $form['edit_skills1'][$i]['skills[]'] = array(
           // '#id' => 'skills[]',
            '#type' => 'textfield',
             // '#required' => TRUE,
            '#title' => t('Skills <span class="form-required">*</span>'),
            '#maxlength' => 50,
            '#prefix' => '<div class="col1">',
            '#suffix' => '</div>'
        );

        $form['edit_skills1'][$i]['proficiency[]'] = array(
            //'#id' => 'proficiency[]',
            '#type' => 'select',
             // '#required' => TRUE,
            '#options' => array('' => 'Select', '1' => 'Beginner', 2 => 'Intermediate', 3 => 'Advanced'),
            '#title' => t('Proficiency <span class="form-required">*</span>'),
            '#maxlength' => 50,
            '#prefix' => '<div class="col2">',
            '#suffix' => '</div>'
        );

        //-- Like wise we can add more
    }
/*
    $form['edit_skills1']['add_name'] = array(
        '#type' => 'submit',
        '#value' => t('Add one more'),
        '#submit' => array('modulename_add_more_add_one'),
        '#prefix' => '<div class="add_more_but"><div class="add_more">',
        '#suffix' => '</div>',
        '#attributes'=>array('class' => array('add_more')),
        // See the examples in ajax_example.module for more details on the
        // properties of #ajax.
        '#ajax' => array(
            'callback' => 'modulename_add_more_callback',
            'wrapper' => 'names-fieldset-wrapper',
        ),
    );

    if ($form_state['num_names'] > 1) {
        $form['edit_skills1']['remove_name'] = array(
            '#type' => 'submit',
            '#value' => t('Remove one'),
            '#attributes' =>array('class' =>array('remove_one')),
            '#submit' => array('modulename_add_more_remove_one'),
            '#ajax' => array(
                'callback' => 'modulename_add_more_callback',
                'wrapper' => 'names-fieldset-wrapper',
            ),
            '#prefix' => '<div class="remove_one">',
            '#suffix' => '</div>',
        );
    }*/
    $form['edit_skills1']['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Submit'),
        '#prefix' => '</div><div class="submit_clear_but">',
    );

     $form['edit_skills1']['cancel_skills'] = array(
        '#type' => 'item', 
        '#markup' => t('<a  class="experience_calcel" href="'.$base_url.'/student/editskills">Cancel</a>'),
        '#suffix' => '</div> </div>',
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
            <a class="active" id="nav_skills" href="'.$base_url.'/student/editskills">
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
            <a id="nav_about_me" href="'.$base_url.'/student/aboutme">
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

    $form['#attributes'] = array('id'=>'student-edit-skills-form');

    return $form;
}

function student_edit_skills_form_submit($form, &$form_state) {
    global $user;
    extract($form_state['input']);

    for ($i = 0; $i < sizeof($skills); $i++) {
        if($skills[$i]!='') {
        $eid = db_insert('student_skills')
            ->fields(array(
                'skills' => $skills[$i],
                'proficiency' => $proficiency[$i],
                'uid' => $user->uid,
                'created_date' => date('Y-m-d H:i:s')
            ))
            ->execute();
        }
    }
    
    audit_log('insert', 'Skills added in manage resume.');
}

/**
 * Callback for both ajax-enabled buttons.
 *
 * Selects and returns the fieldset with the names in it.
 */
function modulename_add_more_callback($form, $form_state) {
    return $form['edit_skills1'];
}

/**
 * Submit handler for the "add-one-more" button.
 *
 * Increments the max counter and causes a rebuild.
 */
function modulename_add_more_add_one($form, &$form_state) {
    $form_state['num_names'] ++;
    $form_state['rebuild'] = TRUE;
}

/**
 * Submit handler for the "remove one" button.
 *
 * Decrements the max counter and causes a form rebuild.
 */
function modulename_add_more_remove_one($form, &$form_state) {
    if ($form_state['num_names'] > 1) {
        $form_state['num_names'] --;
    }
    $form_state['rebuild'] = TRUE;
}

/**
 * Final submit handler.
 *
 * Reports what values were finally set.
 */
function modulename_add_more_submit($form, &$form_state) {

    //Process the data of form here and use it

    /*
      #Example usage
      $form_state['input']['name_fieldset'][0,1,2... index number]['fieldname'];

      you can always use print_r($form_state); to explore the submited data.
     */
}
