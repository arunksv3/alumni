<?php

function student_edit_languages_form($form, &$form_state) {
    global $user;
    global $base_url;
    drupal_add_js(drupal_get_path('theme', 'bootstrap_business') . '/js/jquery.validate.js');
    drupal_add_js(drupal_get_path('theme', 'bootstrap_business') . '/js/jqueryformvalidation.js');
    drupal_add_js(drupal_get_path('module', 'student') . '/js/custom_validations.js');
    drupal_add_css(drupal_get_path('theme', 'bootstrap_business') . '/css/jquery-ui.css');
    drupal_add_js(drupal_get_path('theme', 'bootstrap_business') . '/js/jquery-ui.js');



    $student_languages = db_select('student_languages')
            ->fields('student_languages')
            ->condition('uid', $user->uid, '=')
            ->execute();
    $i = 2;
    
    $count = $student_languages->rowCount(); 
    
    $form = array();
   
   /* $form['edit_languages_link'] = array(
        '#type' => 'fieldset',
        '#title' => t('Edit'),
        '#prefix' => '<div id="EditLanguages" style="cursor: pointer;float:right;width:100px;font-weight:bold;">',
        '#suffix' => '</div>',
    );*/

    $form['edit_languages'] = array(
        '#type' => 'fieldset',
        //'#title' => t('Edit Languages'),
        '#prefix' => '<div id="LanguagesDiv" style="display:block">',
        '#suffix' => '</div>',
    );
    $form['edit_languages']['title'] = array(
        '#type' => 'item',
        '#markup' => '<h2>Languages <span> - Proficiency level: 0 - Poor, 10 - Excellent</span></h2>',         
    );
     $output = ' <ul><li><div id="EditLanguages" class="add-but"> Add </div></li></ul>';
      if($count>0){
     $output.=' <ul><li><table border="0">'
             . '<tr><th>Language</th><th>Written</th><th>Spoken</th><th>Certificate</th><th>Delete</th></tr>';
      while ($row = $student_languages->fetchAssoc($student_languages)) {
        $id = $row['id'];
        $language = $row['language'];
        $spoken = $row['spoken'];
        $written = $row['written'];
        $certificate = $row['certificate'];
        if($row['primary_language']==1)
            $primary = '(Primary)';
        else
            $primary = '';
      $output.=' <tr><td>'.$language.' '.$primary.'</td><td>'.$written.'</td><td>'.$spoken.'</td><td>'.$certificate.'</td>'
              . '<td><a  onclick="return confirm(\'Delete Languages?\')" href="'.$base_url.'/student/deletelanguages/'.encrypt($id,variable_get('IIUM_Encryption_Key', NULL)).'/delete"><i id="icon-delete" class="icon-delete"></i></a></td></tr>';
       }
       
       $output.=' </table></li></ul>';
       
    }
       
        $form['edit_languages']['languages'] = array(
            '#type' => 'item',
             '#markup' => '
                <div id="Education1" class="row" >  
                 
                <div class="col-md-12 resume_mid">
               
                   
                    '.$output.'
                    
                              
                </div>
                 
                </div>
                ',
        );
        $i++;
        
     
 
    $form['edit'] = array(
        '#type' => 'fieldset',        
        '#prefix' => '<div id="EditLanguagesDiv" style="display:none">',
       // '#suffix' => '</div>',
    );
 
    // Because we have many fields with the same values, we have to set
    // #tree to be able to access them.
    $form['edit_languages1']['#tree'] = TRUE;
    $form['edit_languages1'] = array(
        '#type' => 'fieldset',
        '#prefix' => ' '
        . '<div id="names-fieldset-wrapper">',
        '#suffix' => ' </div>',
    );
    $form['edit_languages1']['title'] = array(
        '#type' => 'item',
        '#markup' => '<h2>Add Languages</h2>',         
    );
 
    $form['edit_languages1']['title1'] = array(
          '#type' => 'item',
          '#markup' => 'Proficiency level: 0 - Poor, 10 - Excellent',         
      );
    // Build the fieldset with the proper number of names. We'll use
    // $form_state['num_names'] to determine the number of textfields to build.
    if (empty($form_state['num_names'])) {
        $form_state['num_names'] = 1;
    }
 $numbers = array();
        for($i=10;$i>0;$i--){
            $numbers[$i] = $i;
        }
        //$numbers[0] = 0;
    for ($i = 0; $i < $form_state['num_names']; $i++) {

        //lets add all the fields we want in the set  
        /*
          We have the prefix and suffix added here, so that we can do some sort of styling with the form, like display the fields side by side. You may remove    it, but generally we need that when we have a set of fields, hence I thought to keep it here.
         */

        //Parent container
        $form['edit_languages1'][$i] = array(
            '#prefix' => '<div class="two-col">',
            '#suffix' => '</div>'
        );

        
        

                
        //other form elements
        $form['edit_languages1'][$i]['language[]'] = array(
            '#type' => 'textfield',
            '#title' => t('Languages <span class="form-required">*</span>'),
            '#maxlength' => 50,
            '#prefix' => '<div class="col1">',
            '#suffix' => '</div>'
        );
        
        
        
        $form['edit_languages1'][$i]['primary[]'] = array(
            '#id' => 'searchable',
            '#type' => 'radios',
            '#title' => t('Primary'), 
            '#options' => array(1 => ''),
            //'#required' => TRUE,
            '#default_value' => '',
            '#weight' => 1,
        );
        
       
         $form['edit_languages1'][$i]['spoken[]'] = array(
            '#type' => 'select',
            '#title' => t('Spoken <span class="form-required">*</span>'),
             '#options' => $numbers,
            '#maxlength' => 50,
            '#prefix' => '<div class="col1">',
            '#suffix' => '</div>'
        ); 
         
         
          $form['edit_languages1'][$i]['written[]'] = array(
            '#type' => 'select',
            '#title' => t('Written <span class="form-required">*</span>'),
            '#options' => $numbers,
            '#maxlength' => 50,
            '#prefix' => '<div class="col1">',
            '#suffix' => '</div>'
        );
          
           $form['edit_languages1'][$i]['certificate[]'] = array(
            '#type' => 'textfield',
            '#title' => t('Certificate'),
            '#maxlength' => 50,
            '#prefix' => '<div class="col1">',
            '#suffix' => '</div>'
        );

        //-- Like wise we can add more
    }
    /*
  $form['edit_languages1']['add_name'] = array(
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
        $form['edit_languages1']['remove_name'] = array(
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
    $form['edit_languages1']['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Submit'),
        '#prefix' => '</div><div  class="submit_clear_but">',   
    );
    $form['edit_languages1']['cancel_skills'] = array(
        '#type' => 'item', 
        '#markup' => t('<a  class="experience_calcel" href="'.$base_url.'/student/editlanguages">Cancel</a>'),    
        '#suffix' => '</div> </div>',
    );
    
    
    /*     * *
     * 
     

    $form['edit_languages1']['cancel_languages'] = array(
        '#id' => 'CancelLanguages',
        '#type' => 'fieldset',
        '#title' => t('Cancel'),
        '#prefix' => '<div id="CancelLanguages" style="cursor: pointer;float:right;width:100px;font-weight:bold;background-color:#663398;padding:10px;">',
        '#suffix' => '</div>',
    );*/

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
            <a class="active" id="nav_languages" href="'.$base_url.'/student/editlanguages">
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
   
    $form['#attributes'] = array('id'=>'student-edit-languages-form');

    return $form;
}

function student_edit_languages_form_submit($form, &$form_state) {
    global $user;
    extract($form_state['input']);

    for ($i = 0; $i < sizeof($language); $i++) {
        if(!isset($primary[$i])){
            $primary[$i] = 0;
        }
        if($language[$i]!=''){
        $eid = db_insert('student_languages')
            ->fields(array(
                'language' => $language[$i],
                'written' => $written[$i],
                'spoken' => $spoken[$i],
                'certificate' => $certificate[$i],
                'primary_language' => $primary[$i],
                'uid' => $user->uid,
                'created_date' => date('Y-m-d H:i:s'),
            ))
            ->execute();
        }
    }
    audit_log('insert', 'Languages added in manage resume.');
}

/**
 * Callback for both ajax-enabled buttons.
 *
 * Selects and returns the fieldset with the names in it.
 */
function modulename_add_more_callback($form, $form_state) {
    return $form['edit_languages1'];
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
