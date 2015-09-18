<?php
/**
 * @file
 * A psychometric module that displays basic student deatils.
 */
function psychometrics_demographic($form, &$form_state) { 
    global $user;
    $states_list = alumni_std_states_list();
    
    /* query to fetch from stdinfo */
    $std_query = db_select('stdinfo')
        ->fields('stdinfo')
        ->condition('uid', $user->uid, '=')
        ->execute();
    $std_info = $std_query->fetchAssoc();
    (!empty($std_info)?extract($std_info):'');
    
    /* query to fetch from psychometric_std_info */
    $query = db_select('psychometric_std_info')
        ->fields('psychometric_std_info')
        ->condition('uid', $user->uid, '=')
        ->execute();
    $info = $query->fetchAssoc();    
    
    /* section header links */
    $section_links = get_sections_links();
    
    $form['header_p1'] = array(
        '#type' => 'item',
        '#markup' => $section_links,        
    );
    
    $form['custom_header_div'] = array(
        '#type' => 'item',
        '#prefix' => '<div class="demographic-form">',  
    );
    
    $form['header_p2'] = array(
        '#type' => 'item',
        '#markup' => '<div>Please fill up this section with appropriate information.</div>',
    );
    
    $form['name'] = array(
        '#type' => 'textfield',
        '#title' => t('Name'), 
        '#default_value' => (!empty($name) ? $name : ''),
        '#attributes' => array('readonly' => 'readonly'),
    ); 
     
    $form['matric_no'] = array(
        '#type' => 'textfield',
        '#title' => t('Matric No'), 
        '#default_value' => (!empty($matric_no) ? $matric_no : ''),
        '#attributes' => array('readonly' => 'readonly'),
    ); 
    
    $form['nationality'] = array(
        '#type' => 'textfield',
        '#title' => t('Nationality'),         
        '#default_value' => (!empty($nationality) ? $states_list[$nationality] : ''),
        '#attributes' => array('readonly' => 'readonly'),
    );     

    $form['gender'] = array(
        '#type' => 'radios',
        '#title' => t('Gender'), 
        '#options' => array('Male', 'Female'),  
        '#default_value' => (!empty($gender) ? $gender : ''),
        '#attributes' => array('readonly' => 'readonly'),
    ); 
    
    $form['kulliyyah'] = array(
        '#type' => 'radios',
        '#title' => t('Kulliyyah'), 
        '#options' => kulliyyah(),  
        '#default_value' => (!empty($info['kulliyyah']) ? $info['kulliyyah'] : '')
    );
    
    $form['level_of_study'] = array(
        '#type' => 'radios',
        '#title' => t('Level of Study'), 
        '#options' => student_level_of_study(),
        '#default_value' => (!empty($info['level_of_study']) ? $info['level_of_study'] : '')
    ); 
    
    $form['intake'] = array(
        '#type' => 'radios',
        '#title' => t('Intake'), 
        '#options' => array('Direct','IIUM Centre for Foundation Studies (CFS)'),
        '#default_value' => (!empty($info['intake']) ? $info['intake'] : '')
    ); 
    
    $form['hand_phone_no'] = array(
        '#type' => 'textfield',
        '#title' => t('Hand Phone No'), 
        '#default_value' => (!empty($contact_phone) ? $contact_phone : ''),
        '#attributes' => array('readonly' => 'readonly'),
    ); 
    
    $form['email'] = array(
        '#type' => 'textfield',
        '#title' => t('Email'), 
        '#default_value' => (!empty($alt_email) ? $alt_email : ''),
        '#attributes' => array('readonly' => 'readonly'),
    ); 
    
    $form['buttons']['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Submit'),   
        '#submit' => array('psychometrics_demographic_submit'),
    );
    
    $form['custom_footer_div'] = array(
        '#type' => 'item',
        '#suffix' => '</div>',  
    );
    
    $form['#attributes']['id'][] = 'psychometrics-demographic';
    
    return $form;
}

/* Submit handler for demographic form*/
function psychometrics_demographic_submit($form, &$form_state) { //echo '<pre>'; print_r($form_state['input']); exit;
    global $user;
    
    /* query to fetch from psychometric_std_info */
    $std_query = db_select('psychometric_std_info')
        ->fields('psychometric_std_info')
        ->condition('uid', $user->uid, '=')
        ->execute();
    $std_cnt = $std_query->rowCount();
    
    if($std_cnt > 0){
        $psi = db_update('psychometric_std_info');
        $psi->fields(array(            
            'name' => $form_state['values']['name'],
            'matric_no' => $form_state['values']['matric_no'],
            'nationality' => $form_state['values']['nationality'],
            'gender' => $form_state['values']['gender'],     
            'kulliyyah' => $form_state['values']['kulliyyah'],     
            'level_of_study' => $form_state['values']['level_of_study'],     
            'intake' => $form_state['values']['intake'],     
            'hand_phone_no' => $form_state['values']['hand_phone_no'],     
            'email' => $form_state['values']['email'],           
        ));
        $psi->condition('uid', $user->uid, '=');            
    }else{
        $psi = db_insert('psychometric_std_info');
        $psi->fields(array(
            'uid' => $user->uid,        
            'name' => $form_state['values']['name'],
            'matric_no' => $form_state['values']['matric_no'],
            'nationality' => $form_state['values']['nationality'],
            'gender' => $form_state['values']['gender'],     
            'kulliyyah' => $form_state['values']['kulliyyah'],     
            'level_of_study' => $form_state['values']['level_of_study'],     
            'intake' => $form_state['values']['intake'],     
            'hand_phone_no' => $form_state['values']['hand_phone_no'],     
            'email' => $form_state['values']['email'],     
        ));
        audit_log('Student updated demographic information', $user->uid);
    }
    
    $psi = $psi->execute(); 
    
    if($psi){
        drupal_set_message('You have successfully finished filled the Demographic Information. Please continue with other sections'); 
        drupal_goto('student/psychometrics/demographic_information/rNbTrMQTeYLWk8inNYu1PQ');
    }
}