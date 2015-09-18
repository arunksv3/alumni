<?php
/**
 * @file
 * A psychometric module that displays form to add domain
 */
function psychometrics_add_dom($form, &$form_state, $eid) {  
    $secret_key = variable_get('IIUM_Encryption_Key', NULL);
    $id = (int)decrypt($eid, $secret_key);
    
     /* header links */
    $header_links = admin_header_links();    
     
    if($eid!="0"){
        /* query to fetch psychometric_domains */
        $dom_query = db_select('psychometric_domains')
            ->fields('psychometric_domains')
            ->condition('id', $id, '=')
            ->execute();
        $dom_info = $dom_query->fetchAssoc();
        (!empty($dom_info)?extract($dom_info):'');
    }
    
     /* query to fetch psychometrics sections */
    $sec_query = db_select('psychometric_sections')
        ->fields('psychometric_sections')
        ->condition('delete_status', '0', '=')
        ->execute();
    $sec_result = $sec_query->fetchAll();
    
    $result = array();
    foreach ($sec_result as $value) {
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
    
    $form['desc'] = array(
        '#type' => 'text_format',       
        '#format' => filter_default_format(),
        '#title' => t('Description'), 
        '#default_value' => (!empty($desc) ? $desc : '')
    );  
    
    $form['low_meanings'] = array(
        '#type' => 'text_format',       
        '#format' => filter_default_format(),
        '#title' => t('Low Meanings'), 
        '#default_value' => (!empty($low_meanings) ? $low_meanings : '')
    );
    
    $form['moderate'] = array(
        '#type' => 'text_format',       
        '#format' => filter_default_format(),
        '#title' => t('Moderate'), 
        '#default_value' => (!empty($moderate) ? $moderate : '')
    );
    
    $form['high_meanings'] = array(
        '#type' => 'text_format',       
        '#format' => filter_default_format(),
        '#title' => t('Description'), 
        '#default_value' => (!empty($high_meanings) ? $high_meanings : '')
    );
    
     $form['eid'] = array(
        '#type' => 'hidden',        
        '#default_value' => $eid
    );  
    
    $form['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Submit'),   
        '#submit' => array('psychometrics_add_dom_submit'),
    );
    
    return $form;
}

/* Submit handler for add section form*/
function psychometrics_add_dom_submit($form, &$form_state) {   //echo '<pre>'; print_r($form_state['values']); exit; 
    global $user;
    $uri = admin_uri();
    $secret_key = variable_get('IIUM_Encryption_Key', NULL);
    $eid = (int)decrypt($form_state['values']['eid'], $secret_key); 
    
    if($eid!=0){
        $ps = db_update('psychometric_domains');
        $ps->fields(array(       
            'secid' => $form_state['values']['section'],
            'title' => $form_state['values']['title'],
            'descrption' => $form_state['values']['descrption']['value'],            
            'low_meanings' => $form_state['values']['low_meanings']['value'],
            'moderate' => $form_state['values']['moderate']['value'],
            'high_meanings' => $form_state['values']['high_meanings']['value'],
        ));
        $ps->condition('id', $eid, '=');                    
    }else{
        $ps = db_insert('psychometric_domains');
        $ps->fields(array(       
            'secid' => $form_state['values']['section'],
            'title' => $form_state['values']['title'],
            'descrption' => $form_state['values']['descrption']['value'],            
            'low_meanings' => $form_state['values']['low_meanings']['value'],
            'moderate' => $form_state['values']['moderate']['value'],
            'high_meanings' => $form_state['values']['high_meanings']['value'],     
        ));
        audit_log('Psychometric domains added', $user->uid);
    }
    //echo $ps; exit;
    $ps = $ps->execute(); 
    if($ps){
        drupal_set_message('You have successfully created the domain.'); 
        drupal_goto($uri.'domains');
    }
}