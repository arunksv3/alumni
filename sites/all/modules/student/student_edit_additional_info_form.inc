<?php 
function student_edit_additional_info_form($form, &$form_state) {
    global $user;
    global $base_url;
    drupal_add_js(drupal_get_path('theme', 'bootstrap_business') .'/js/jquery.validate.js');    
    drupal_add_js(drupal_get_path('theme', 'bootstrap_business') .'/js/jqueryformvalidation.js');
    drupal_add_js(drupal_get_path('module', 'student') .'/js/custom_validations.js');
    drupal_add_css(drupal_get_path('theme', 'bootstrap_business') .'/css/jquery-ui.css');  
    drupal_add_js(drupal_get_path('theme', 'bootstrap_business') .'/js/jquery-ui.js');
    
     
    
     $student_additional_info = db_select('student_details')
    ->fields('student_details')    
    ->condition('uid', $user->uid, '=')
 ->execute();
     
$form = array();
$i=0;
  $count = $student_additional_info->rowCount();
 
      
    $form['additional_info1'] = array(
        '#type' => 'fieldset', 
       // '#title' => t('Edit Additional Info'), 
       // '#prefix' => '<div id="additionalInfo" style="display:block">',
       // '#suffix' => '</div>',
    ); 
    $form['additional_info1']['title'] = array(
        '#type' => 'item', 
        '#markup' => '<h2>Additional Info</h2>',        
    ); 
     $form['additional_info1']['edit_additional_info'] = array(
        '#id' => 'EditAdditionalInfo',
        '#type' => 'fieldset', 
        '#title' => t('<i id="icon-edit" class="icon-edit"></i>'), 
        '#prefix' => '<div id="EditAdditionalInfo" style="cursor: pointer;float:right;">',
        '#suffix' => '</div>',
        ); 
     $country = 0;
     if($count>0) { 
    while($row = $student_additional_info->fetchAssoc($student_additional_info)){
        $expected_salary = $row['expected_salary'];
        $country = $row['location_country'];
        $state = $row['location_state'];
        $preferred_country = get_country_name($row['location_country']);
        $preferred_state = get_state_name($row['location_state']); 
        $other_info= $row['additional_info']; 
    
        $form['additional_info1' . $i]['info'] = array(
            '#type' => 'item',
           // '#title' => t(isset($proficiency) ? $proficiency . ':' : '' . ':'),
            '#markup' => ' 
                <div id="additionalInfo" class="row" >                 
                    <div class="col-md-10 resume_mid">
                        <ul class="additional_info">
                            <li><label>Expected Salary (MYR) : </label><span>'.$expected_salary.'</span></li>
                            <li><label>Preferred Work Location: </label><span>'.$preferred_country.' '.$preferred_state.'</span></li>                            
                            <li><label>Other Information: </label><span>'.$other_info.'</span></li>
                        </ul>                
                    </div>
                    <div class="col-md-2 resume_right"> </div>
                </div>
                 
                ',
        );
        $i++;
        
        
    }
    
  }
    
    
    
    // To make these fields required
     
     
     
   
    /*
    $form['additional_info1']['expected_salary'] = array(
        '#type' => 'item',
        '#title' => t('Expected Salary:'),
         '#markup' => (isset($expected_salary) ? $expected_salary : ''),  
    );

    $form['additional_info1']['preferred_country'] = array(
         '#id' => 'country',
        '#type' => 'item',
        '#title' => t('Country:'),         
        '#markup' => (isset($preferred_country) ? $preferred_country : ''),
    );
    
      $form['additional_info1']['preferred_state'] = array(
         '#id' => 'state:',
        '#type' => 'item',
        '#title' => t('State:'),         
        '#markup' => (isset($preferred_state) ? $preferred_state : ''),
    );
    

    $form['additional_info1']['other_info'] = array(
     '#type' => 'item', 
     '#title' => 'Other Information:',
      '#markup' => (isset($other_info) ? $other_info : ''),  
     );*/
    
    
    
    $form['additional_info'] = array(
        '#type' => 'fieldset', 
        //'#title' => t('Edit Additional Info'), 
        '#prefix' => '<div id="additionalInfoDiv" style="display:none">',
        '#suffix' => '</div>',
    );   
    /*$form['additional_info']['title'] = array(
        '#type' => 'item', 
        '#markup' => '<h2>Edit Additional Info</h2>',        
    ); */
    $form['additional_info']['expected_salary'] = array(
        '#id'=>'expected_salary',
        '#type' => 'textfield',
          //'#required' => TRUE,
        '#title' => t('Expected Salary (MYR) <span class="form-required">*</span>'),
         '#default_value' => (isset($expected_salary) ? $expected_salary : ''),  
    );
     $form['additional_info']['preferred_country'] = array(
         '#id' => 'preferred_country',
        '#type' => 'select',
          // '#required' => TRUE,
        '#title' => t('Preferred Work Location <span class="form-required">*</span>'),
        '#options' => array(''=>'Select') + countries_list(),
        '#ajax' => array(
            'event' => 'change',
            'callback' => 'student_additional_state_list_ajax',
            'wrapper' => 'state-list-wrapper',
        ),
        '#default_value' => (isset($country) ? $country : ''),
    );
    
    // Wrapper for state dropdown list
    $form['additional_info']['address']['wrapper'] = array(
        '#prefix' => '<div id="state-list-wrapper">',
        '#suffix' => '</div>',
    );
    
    if ((isset($form_state['values']['preferred_country']) && $form_state['values']['preferred_country'] != "") || $country>0) {        
        $cid = isset($form_state['values']['preferred_country'])?$form_state['values']['preferred_country']:$country;        
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
            $form['additional_info']['address']['wrapper']['preferred_state'] = array(
                '#id' => 'preferred_state',
                '#type' => 'select',
                '#title' => t('State <span class="form-required">*</span>'),
                '#multiple' => false,
                '#options' => $state_list_arr,
                '#default_value' => (isset($state) ? $state : ''),                
            );
        }
    }
    

    $form['additional_info']['other_info'] = array(
     '#type' => 'text_format', 
     '#title' => 'Other Information',
      '#default_value' => (isset($other_info) ? $other_info : ''),  
     );
       
         
        $form['additional_info']['submit'] = array(
            '#type' => 'submit',
            '#value' => t('Submit'),   
            '#prefix' => '<div class="submit_clear_but">',
            //'#suffix' => '</div>',
        );
      
        $form['additional_info']['cancel_skills'] = array(
        '#id' => 'Cancel',
        '#type' => 'button',
        '#value' => t('Cancel'),         
        '#suffix' => '</div>',
    );
       /* $form['additional_info']['cancel_additional_info'] = array(
        '#id' => 'CancelAdditionalInfo',
        '#type' => 'fieldset', 
        '#title' => t('Cancel'), 
        '#prefix' => '<div id="EditAdditionalInfo" style="cursor: pointer;float:right;width:100px;font-weight:bold;background-color:#663398;padding:10px;">',
        '#suffix' => '</div>',
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
            <a class="active" id="nav_add_info" href="'.$base_url.'/student/editadditionalinfo">
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
    $form['#attributes'] = array('enctype' => 'multipart/form-data');
    
    return $form;  
}

function student_edit_additional_info_form_submit($form, &$form_state) {
    global $user;
     extract($form_state['input']);
      if(!isset($preferred_state) || $preferred_state==''){
           $preferred_state = 0;
      }
    $student_details = db_select('student_details')
        ->fields('student_details')    
        ->condition('uid', $user->uid, '=')
        ->execute();  
    $count = $student_details->rowCount(); 
    if($count>0) {
        $eid = db_update('student_details')
        ->fields(array(
            'expected_salary' => $expected_salary,
            'location_country' => $preferred_country,
	    'location_state' => $preferred_state,
            'additional_info' => $other_info['value'],
          ))
        ->condition('uid', $user->uid, '=')
         ->execute();
    } else {
        $eid = db_insert('student_details')
        ->fields(array(
            'expected_salary' => $expected_salary,
            'location_country' => $preferred_country,
	    'location_state' => $preferred_state,
            'additional_info' => $other_info['value'],
             'uid' => $user->uid,
            'created_date' => date('Y-m-d H:i:s')
        ))
        ->execute();
    }
    
     audit_log('update', 'Additional Info updated in manage resume.');
}