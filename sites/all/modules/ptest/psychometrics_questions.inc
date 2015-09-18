<?php
/**
 * @file
 * A psychometric module that displays questions based on sections
 */
function psychometrics_questions() { 
    $uri = admin_uri();
    $secret_key = variable_get('IIUM_Encryption_Key', NULL);
     
    /* header links */
    $output = admin_header_links();
    
    $output .= l('Add Question', $uri.'questions/add_qst/'.  encrypt('0', $secret_key));
    
    /* query to fetch psychometrics questions */
    $query = db_select('psychometric_questions')
            ->fields('psychometric_questions')
            ->condition('delete_status', '0', '=')  
            ->orderBy('id', 'Desc')
            ->execute();
        
    $result = $query->fetchAll();
    
    /* Table header columns */
    $header = array(
        array('data' => t('Section Title')), 
        array('data' => t('Domain Title')), 
        array('data' => t('Question')), 
        array('data' => t('Reverse Score')), 
        array('data' => t('Options')), 
    );

    $result_arr = array();    
    foreach($result as $value){                     
        $result_arr[] = array(get_section_name($value->secid), get_domain_name($value->domid), $value->question, ($value->reverse_score=='0'?'Yes':'No'), l('View', $uri.'view_qst/'.encrypt($value->id,$secret_key), array('attributes' => array('class' => array('view-section','simple-dialog'), 'rel' => array('width:900;resizable:false;')) )).' '. l('Edit', $uri.'questions/add_qst/'.encrypt($value->id,$secret_key), array('attributes' => array('class' => array('edit-section')))) );
    }
    
    // pagination script starts    
    $per_page = 10;
    // Initialize the pager
    $current_page = pager_default_initialize(count($result_arr), $per_page);
    // Split your list into page sized chunks
    $chunks = array_chunk($result_arr, $per_page, TRUE);
    // Show the appropriate items from the list
    $output .= theme('table', array('header' => $header, 'rows' => $chunks[$current_page], 'empty' => t('No Records Found!')));
    // Show the pager
    $output .= theme('pager', array('quantity',count($result_arr)));   

    return $output;
    
}