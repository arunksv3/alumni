<?php
/**
 * @file
 * A psychometric module that displays form to attend for test.
 */
function psychometrics_admin() { 
     /* header links */    
    $uri = admin_uri(); 
    $output = admin_header_links(); 
    $secret_key = variable_get('IIUM_Encryption_Key', NULL);
    
    /* query to fetch psychometrics std info */
    $query = db_select('psychometric_std_info')  
            ->fields('psychometric_std_info')                       
            ->orderBy('id', 'Desc')
            ->execute();
        
    $std_info = $query->fetchAll();
    
    /* Table header columns */
    $header = array(
        array('data' => t('Matric No')), 
        array('data' => t('Name')), 
        array('data' => t('Email')), 
        array('data' => t('Gender')),
        array('data' => t('Options')),
    );

    $result_arr = array();    
    $g_status = gender_status();
    foreach($std_info as $value){     
        $result_arr[] = array($value->matric_no, $value->name, $value->email, $g_status[$value->gender], l('View Results', $uri.'results/'.encrypt($value->uid,$secret_key), array('attributes' => array('class' => array('view-ptest-res')))));
    }
    
    // pagination script starts    
    $per_page = 5;
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