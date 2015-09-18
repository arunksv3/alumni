<?php
/**
 * @file
 * A psychometric module that displays form to add question
 */
function psychometrics_add_qst($form, &$form_state, $eid) {  
    $secret_key = variable_get('IIUM_Encryption_Key', NULL);
    $id = (int)decrypt($eid, $secret_key);
    
     /* header links */
    $header_links = admin_header_links();    
     
    if($eid!="0"){
        /* query to fetch psychometric_questions */
        $qst_query = db_select('psychometric_questions')
            ->fields('psychometric_questions')
            ->condition('id', $id, '=')
            ->condition('delete_status', '0', '=')
            ->execute();
        $qst_info = $qst_query->fetchAssoc();
        (!empty($qst_info)?extract($qst_info):'');
        
        /* inline javascript */
        drupal_add_js(' 
            jQuery(document).ready(function($) {
                $("#edit-secid").change();  
            });
        ', 'inline');
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
     
    $form['secid'] = array(
        '#type' => 'select',
        '#title' => t('Sections'),
        '#options' => array(''=>'Select') + $result,
        '#required' => TRUE,
        '#default_value' => (!empty($secid) ? $secid : ''),
        '#ajax' => array(
            'event' => 'change',
            'callback' => 'psychometrics_list_ajax',
            'wrapper' => 'psychometrics-list-wrapper',
        ),
    ); 
    
    // Wrapper for sub parent dropdown list
    $form['wrapper'] = array(
        '#prefix' => '<div id="psychometrics-list-wrapper">',
        '#suffix' => '</div>',
    );
    
    if (isset($form_state['values']['secid']) && $form_state['values']['secid']!="") {
        $section_id = $form_state['values']['secid'];
        /* query to fetch psychometrics domains */
        $dom_query = db_select('psychometric_domains')
                ->fields('psychometric_domains')
                ->condition('secid', $section_id, '=') 
                ->orderBy('id', 'Desc')
                ->execute();

        $dom_result = $dom_query->fetchAll();

        $resultArr = array();
        foreach ($dom_result as $value) {
            $resultArr[$value->id] = $value->title; 
        }
        
        $form['wrapper']['domid'] = array(
            '#type' => 'select',
            '#title' => t('Domains'),
            '#required' => TRUE,
            '#options' => $resultArr,            
            '#default_value' => (isset($domid) ? $domid : ''),
        );
    }
    
    $form['question'] = array(
        '#type' => 'textfield',
        '#title' => t('Question'), 
        '#default_value' => (!empty($question) ? $question : '')
    ); 
    
    $form['reverse_score'] = array(
        '#type' => 'radios',       
        '#title' => t('Reverse Score'),  
        '#options' => array(t('Yes'), t('No')), 
        '#default_value' => (!empty($reverse_score) ? $reverse_score : '')
    );   
    
     $form['eid'] = array(
        '#type' => 'hidden',        
        '#default_value' => $eid
    );  
    
    $form['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Submit'),   
        '#submit' => array('psychometrics_add_qst_submit'),
    );
    
    return $form;
}

/* Submit handler for add section form*/
function psychometrics_add_qst_submit($form, &$form_state) {   //echo '<pre>'; print_r($form_state['values']); exit;
    global $user;
    $uri = admin_uri();
    $secret_key = variable_get('IIUM_Encryption_Key', NULL);
    $eid = (int)decrypt($form_state['values']['eid'], $secret_key); 
    
    /* query to fetch psychometrics sections */
     /* max id of questions based on section */ 
    $maxquery = db_select('psychometric_questions');
    $maxquery->condition('secid', $form_state['values']['secid'], '=');
    $maxquery->addExpression('MAX(qnum)');
    $max = $maxquery->execute()->fetchField(); 
    
    if($eid!=0){
        $pq = db_update('psychometric_questions');
        $pq->fields(array(       
            'secid' => $form_state['values']['secid'],
            'domid' => $form_state['values']['domid'],   
            'question' => $form_state['values']['question'],   
            'reverse_score' => $form_state['values']['reverse_score'],   
        ));
        $pq->condition('id', $eid, '=');                    
    }else{
        $pq = db_insert('psychometric_questions');
        $pq->fields(array(  
            'qnum' => $max,
            'secid' => $form_state['values']['secid'],
            'domid' => $form_state['values']['domid'],   
            'question' => $form_state['values']['question'],   
            'reverse_score' => $form_state['values']['reverse_score'],        
        ));
        audit_log('Psychometric question added', $user->uid);
    }
    //echo $pq; exit;
    $pq = $pq->execute(); 
    if($pq){
        drupal_set_message('You have successfully created the section.'); 
        drupal_goto($uri.'questions');
    }
}


/**
 * Function for handling ajax request
 */
function psychometrics_list_ajax($form, $form_state) {
    // Return the dropdown list including the wrapper
    return $form['wrapper'];
}