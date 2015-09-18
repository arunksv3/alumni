<?php
/**
 * @file
 * A psychometric module that displays form to add domain
 */
function psychometrics_add_scale($form, &$form_state, $eid) {  
    $secret_key = variable_get('IIUM_Encryption_Key', NULL);
    $id = (int)decrypt($eid, $secret_key);
    
     /* header links */
    $header_links = admin_header_links();    
     
    if($eid!="0"){
        /* query to fetch psychometric_domains */
        $scale_query = db_select('psychometric_scales')
            ->fields('psychometric_scales')
            ->condition('id', $id, '=')
            ->condition('delete_status', '0', '=')
            ->execute();
        $scale_info = $scale_query->fetchAssoc();
        (!empty($scale_info)?extract($scale_info):'');
    }
    
     /* query to fetch psychometrics sections */
    $sec_query = db_select('psychometric_sections')
        ->fields('psychometric_sections')
        ->condition('delete_status', '0', '=')
        ->execute();
    $sec_result = $sec_query->fetchAll();
    
    $result = array();
    foreach ($sec_result as $value) {
        if($value->id!='1')
            $result[$value->id] = $value->title; 
    }
    
    $form['header'] = array(
        '#type' => 'item',
        '#markup' => $header_links,        
    );
    
    $form['section'] = array(
        '#type' => 'select',
        '#title' => t('Sections'),
        '#options' => array(''=>'Select') + $result,
        '#required' => TRUE,
        '#default_value' => (!empty($secid) ? $secid : '')
    ); 
     
    $form['title'] = array(
        '#type' => 'textfield',
        '#title' => t('Title'), 
        '#default_value' => (!empty($title) ? $title : '')
    );     
    
    $form['eid'] = array(
        '#type' => 'hidden',        
        '#default_value' => $eid
    );  
    
    $form['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Submit'),   
        '#submit' => array('psychometrics_add_scale_submit'),
    );
    
    return $form;
}

/* Submit handler for add section form*/
function psychometrics_add_scale_submit($form, &$form_state) {   //echo '<pre>'; print_r($form_state['values']); exit; 
    global $user;
    $uri = admin_uri();
    $secret_key = variable_get('IIUM_Encryption_Key', NULL);
    $eid = (int)decrypt($form_state['values']['eid'], $secret_key); 
    
    if($eid!=0){
        $ps = db_update('psychometric_scales');
        $ps->fields(array(       
            'secid' => $form_state['values']['section'],
            'title' => $form_state['values']['title'],           
        ));
        $ps->condition('id', $eid, '=');                    
    }else{
        $ps = db_insert('psychometric_scales');
        $ps->fields(array(       
            'secid' => $form_state['values']['section'],
            'title' => $form_state['values']['title'],              
        ));
        audit_log('Psychometric scales added', $user->uid);
    }
    //echo $ps; exit;
    $ps = $ps->execute(); 
    if($ps){
        drupal_set_message('You have successfully created the domain.'); 
        drupal_goto($uri.'scales');
    }
}