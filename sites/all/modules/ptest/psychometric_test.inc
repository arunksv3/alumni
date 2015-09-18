<?php
/**
 * @file
 * A psychometric module that displays form to attend for test.
 */
function psychometrics_test($form, &$form_state, $id) {   
    global $user;
    drupal_add_js(drupal_get_path('theme', 'bootstrap_business') .'/js/jquery.validate.js');    
    drupal_add_js(drupal_get_path('theme', 'bootstrap_business') .'/js/jqueryformvalidation.js');
    drupal_add_js(drupal_get_path('module', 'ptest') .'/js/custom_validations.js');
    
    $secret_key = variable_get('IIUM_Encryption_Key', NULL);
    $secid = (int)decrypt($id, $secret_key);   
    $revere_scoring_cats = array('1');
    
    /* query to fetch psychometrics sections based on secid */
    $query = db_select('psychometric_sections')
        ->fields('psychometric_sections', array('id','description'))
        ->condition('delete_status', '0', '=')
        ->condition('id', $secid, '=')
        ->execute();
    $result = $query->fetchAssoc(); 
    
    /* section header links */
    $section_links = get_sections_links();    
    
    $form['#attributes']['class'][] = 'ptestforms';
    $form['#attributes']['id'][] = 'psychometrics-test';
    
    $form['header_p1'] = array(
        '#type' => 'item',
        '#markup' => $section_links,        
    );
    
    $form['custom_header_div'] = array(
        '#type' => 'item',
        '#prefix' => '<div class="sec-wis-qst">',  
    );
        
    /* Check if user already attend the test */
    $ptquery = psychometric_test_check($secid, $user->uid);
    $p_test = $ptquery->rowCount();
    $p_mode = $ptquery->fetchColumn('2'); 
    
    /* query to fetch psychometrics questions based on section id */
    $pq_query = psychometric_questions($secid);
    $pq_results = $pq_query->fetchAll(); 
    
    $ps = db_select('psychometric_scales','ps');    
    $ps->fields('ps');
    $ps->condition('delete_status', '0', '=');
    $ps->condition('secid', $secid, '=');
    $ps_query = $ps->execute();
    $ps_results = $ps_query->fetchAll();
     
    $scales = '<div class="scale-sec"><ul class="scales">';  
    foreach ($ps_results as $key => $value) { 
        $scales .= '<li>'.$value->title.'</li>';  
    }
    $scales .= '</ul></div>';   
    
    $form['header_p2'] = array(
        '#type' => 'item',
        '#markup' => $scales,        
    );
    
    $scales_count = count($ps_results);
    $scales_no_label = array();
    for($i=1; $i<=$scales_count; $i++){
        array_unshift($scales_no_label,"");
        unset($scales_no_label[0]);
        $scales_no_label[] = '';
    }
    
    $i = 1;
    foreach ($pq_results as $value) { 
        if($i%2==0)
            $class = 'even';
        else
            $class = 'odd';
        
        if($p_mode != '1' && $p_mode != ''){
            /* display answer if already attend the test */
            $pt_query = db_select('psychometric_test')
                ->fields('psychometric_test')            
                ->condition('qnum', $value->qnum, '=')            
                ->orderBy('id', 'DESC')
                ->execute();
            $pt_result = $pt_query->fetchAssoc();  
        }
        
        $form['qnum_'.$value->qnum] = array(
            '#type' => 'item', 
            '#markup' => $value->qnum.'.',
            '#prefix' => '<div class="question-sec '.$class.'"><div class="qnum">',
            '#suffix' => '</div>',
        );
            
        $form['questions_'.$value->qnum] = array(
            '#type' => 'radios',
            '#title' => t($value->question), 
            '#options' => $scales_no_label,  
            '#default_value' => (!empty($pt_result) ? ($pt_result['answer']!=0?$pt_result['answer']:NULL) : NULL),
            '#suffix' => '</div>',
        ); 
        
        $form['domain_'.$value->qnum] = array(
            '#type' => 'hidden',            
            '#value' => encrypt($value->domid, $secret_key),          
        );
        
        if(in_array($result['id'], $revere_scoring_cats)){
            $form['reverse_score_'.$value->qnum] = array(
                '#type' => 'hidden',            
                '#value' => encrypt($value->reverse_score, $secret_key),          
            ); 
        }
        
        $form['attended_test'] = array(
            '#type' => 'hidden',            
            '#value' => $p_test,          
        ); 
        
        $i++;
    }   
    
    $form['secid'] = array(
        '#type' => 'hidden', 
        '#value' => $id
    );
    
    $form['buttons'] = array(
        '#type' => 'fieldset', 
    );
    
    $form['buttons']['save'] = array(
        '#type' => 'submit',        
        '#value' => t('Save Test'),         
        '#submit' => array('psychometrics_test_submit'),
    );
    
    if($p_mode != "" && $p_mode == "0"){
        $form['buttons']['retake'] = array(
            '#type' => 'submit',
            '#value' => t('Retake Test'),   
            '#submit' => array('psychometrics_test_submit'),
        );
    }
    
    $form['buttons']['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Submit Test'),   
        '#submit' => array('psychometrics_test_submit'),
    );
    
    $form['custom_footer_div'] = array(
        '#type' => 'item',
        '#suffix' => '</div>',  
    );
    
    return $form;
}

/* psychometrics_test submission method */
function psychometrics_test_submit($form, &$form_state){ 
    global $user;
    $secret_key = variable_get('IIUM_Encryption_Key', NULL);
    $secid = (int)decrypt($form_state['input']['secid'], $secret_key);
    
    /* min id of questions based on section */
    $minquery = db_select('psychometric_questions');
    $minquery->condition('secid', $secid, '=');
    $minquery->addExpression('MIN(qnum)');
    $min = $minquery->execute()->fetchField(); 
    
    /* max id of questions based on section */ 
    $maxquery = db_select('psychometric_questions');
    $maxquery->condition('secid', $secid, '=');
    $maxquery->addExpression('MAX(qnum)');
    $max = $maxquery->execute()->fetchField(); 
    
    /* Check if user already attend the test */
    $ptquery = psychometric_test_check($secid, $user->uid);
    $p_mode = $ptquery->fetchColumn('2'); 
    
    if($form_state['input']['op'] != 'Submit Test')
        $mode = "0";
    else
        $mode = "1";
    
    if($form_state['input']['op'] != 'Retake Test'){
        if($form_state['input']['attended_test'] > 0 && $p_mode == '0'){
            $pi = db_update('psychometric_info');
            $pi->fields(array(                      
                'mode' => $mode,
            ));
            $pi->condition('uid', $user->uid, '=');
            $pi->condition('secid', $secid, '=');
        }else{
            $pi = db_insert('psychometric_info');
            $pi->fields(array(
                'uid' => $user->uid,          
                'secid' => $secid,
                'mode' => $mode,
            ));
            audit_log('Student attended psychometric test', $user->uid);
        }
        $pi = $pi->execute();
    }
    
    /* Update pt_id */
    if ($pi <= 9)
        $value = "000" . $pi;
    else if ($pi <= 99 && $pi > 9)
        $value = "00" . $pi;
    else if ($pi <= 999 && $pi > 99)
        $value = "0" . $pi;
    else
        $value = $pi;

    $pt_id = 'PT-' . $value;

    $fields = array(
        'pt_id' => $pt_id,
    );
    
    db_update('psychometric_info')
        ->fields($fields)
        ->condition('id', $pi)
        ->execute();    
    
    // inserting/updating question and answers to psychometrics_test table
    for($i=$min; $i<=$max; $i++){ 
        $answer = $form_state['input']['questions_'.$i];
        @$domid = (int)decrypt($form_state['input']['domain_'.$i], $secret_key);    
        
        if($form_state['input']['attended_test'] > 0 && $p_mode == '0' && $form_state['input']['op'] != 'Retake Test'){  
            $pt = db_update('psychometric_test');
            $pt->fields(array(                                                
                'answer' => $answer,                
            ));        
            $pt->condition('qnum', $i, '=');
        }else if($form_state['input']['op'] == 'Retake Test'){ 
            $pt = db_update('psychometric_test');
            $pt->fields(array(                                                
                'answer' => '0',
            ));        
            $pt->condition('qnum', $i, '=');
        }else{
            $pt = db_insert('psychometric_test');
            $pt->fields(array(
                'pid' => $pi,
                'domid' => $domid,
                'qnum' => $i,
                'answer' => $answer,
            ));
        }
        $pt = $pt->execute();  
    } 
    
    if($form_state['input']['op'] == 'Submit Test'){
        drupal_set_message('Your result will be saved to "View Results" tab for fruther references'); 
        drupal_goto('student/psychometrics_result/'.encrypt($secid,$secret_key).'/'.encrypt($pi,$secret_key));
    }else if($form_state['input']['op'] == 'Retake Test'){
        drupal_goto(current_path());
    }else{
        drupal_set_message('You test is incomplete and saved to our system. You can come later and finish this test. Thank you for your interest.');         
    }
}