<?php
/**
 * @file
 * A psychometric module that displays domains based on sections
 */
function psychometrics_scales() { 
    $uri = admin_uri();
    $secret_key = variable_get('IIUM_Encryption_Key', NULL);
     
    /* header links */
    $output = admin_header_links();
    
    $output .= l('Add Scales', $uri.'scales/add_scale/'.  encrypt('0', $secret_key));
    
    /* query to fetch psychometrics domains */
    $query = db_select('psychometric_scales')
            ->fields('psychometric_scales')
            ->condition('delete_status', '0', '=')  
            ->orderBy('id', 'Desc')
            ->execute();
        
    $result = $query->fetchAll();
    
    /* Table header columns */
    $header = array(
        array('data' => t('Section Title')), 
        array('data' => t('Scale Title')), 
        array('data' => t('Options')), 
    );

    $result_arr = array();    
    foreach($result as $value){                     
        $result_arr[] = array(get_section_name($value->secid), $value->title,  l('View', $uri.'view_scale/'.encrypt($value->id,$secret_key), array('attributes' => array('class' => array('view-section','simple-dialog'), 'rel' => array('width:900;resizable:false;')) )).' '. l('Edit', $uri.'scales/add_scale/'.encrypt($value->id,$secret_key), array('attributes' => array('class' => array('edit-section')))) );
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