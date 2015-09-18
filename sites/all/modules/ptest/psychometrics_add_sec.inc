<?php
/**
 * @file
 * A psychometric module that displays form to add section
 */
function psychometrics_add_sec($form, &$form_state, $eid) {  
    $secret_key = variable_get('IIUM_Encryption_Key', NULL);
    $id = (int)decrypt($eid, $secret_key);
    
     /* header links */
    $header_links = admin_header_links();    
     
    if($eid!="0"){
        /* query to fetch psychometric_sections */
        $sec_query = db_select('psychometric_sections')
            ->fields('psychometric_sections')
            ->condition('id', $id, '=')
            ->condition('delete_status', '0', '=')
            ->execute();
        $sec_info = $sec_query->fetchAssoc();
        (!empty($sec_info)?extract($sec_info):'');
    }
    
    $form['header'] = array(
        '#type' => 'item',
        '#markup' => $header_links,        
    );
     
    $form['title'] = array(
        '#type' => 'textfield',
        '#title' => t('Title'), 
        '#default_value' => (!empty($title) ? $title : '')
    ); 
    
    $form['description'] = array(
        '#type' => 'text_format',       
        '#format' => filter_default_format(),
        '#title' => t('Description'), 
        '#default_value' => (!empty($description) ? $description : '')
    );  
    
     $form['eid'] = array(
        '#type' => 'hidden',        
        '#default_value' => $eid
    );  
    
    $form['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Submit'),   
        '#submit' => array('psychometrics_add_sec_submit'),
    );
    
    return $form;
}

/* Submit handler for add section form*/
function psychometrics_add_sec_submit($form, &$form_state) {   //echo '<pre>'; print_r($form_state['values']); exit;
    global $user;
    $uri = admin_uri();
    $secret_key = variable_get('IIUM_Encryption_Key', NULL);
    $eid = (int)decrypt($form_state['values']['eid'], $secret_key); 
    
    if($eid!=0){
        $ps = db_update('psychometric_sections');
        $ps->fields(array(       
            'title' => $form_state['values']['title'],
            'description' => $form_state['values']['description']['value'],        
        ));
        $ps->condition('id', $eid, '=');                    
    }else{
        $ps = db_insert('psychometric_sections');
        $ps->fields(array(       
            'title' => $form_state['values']['title'],
            'description' => $form_state['values']['description']['value'],        
        ));
        audit_log('Psychometric sections added', $user->uid);
    }
    //echo $ps; exit;
    $ps = $ps->execute(); 
    if($ps){
        drupal_set_message('You have successfully created the section.'); 
        drupal_goto($uri.'sections');
    }
}