<?php
/**
 * @file
 * A psychometric module that displays form to attend for test.
 */
function psychometricsresults($userid) { 
    global $user;
    $uri = admin_uri(); 
    $secret_key = variable_get('IIUM_Encryption_Key', NULL);
    
    if($user->uid == '1'){
        $userid = (int)decrypt($userid,$secret_key);
        $url = $uri;
    }else{
        $userid = $user->uid;
        $url = 'student/psychometrics_result/';
    }
    
    /* query to fetch psychometrics test based on id */
    $query = db_select('psychometric_info')    
            ->condition('uid', $userid, '=')
            ->condition('mode', '1', '=')
            ->fields('psychometric_info')
            ->orderBy('id', 'Desc')
            ->execute();
        
    $result = $query->fetchAll();
    
    /* Table header columns */
    $header = array(
        //array('data' => t('Test Id')), 
        array('data' => t('Test Completed On')), 
        array('data' => t('Test Name')),
        array('data' => t('Options')),
    );

    $result_arr = array();    
    foreach($result as $value){     
        $dateArr = explode(' ', $value->created_date);
        $test_date = dateformat($dateArr['0']); //$value->pt_id
        $result_arr[] = array($test_date, get_section_name($value->secid), l('View Results', $url.encrypt($value->secid,$secret_key).'/'.encrypt($value->id,$secret_key), array('attributes' => array('class' => array('view-ptest-res')))));
    }
    
    // pagination script starts    
    $per_page = 10;
    // Initialize the pager
    $current_page = pager_default_initialize(count($result_arr), $per_page);
    // Split your list into page sized chunks
    $chunks = array_chunk($result_arr, $per_page, TRUE);
    // Show the appropriate items from the list
    $output =  '<div class="res-tab">' .theme('table', array('header' => $header, 'rows' => $chunks[$current_page], 'empty' => t('No Records Found!'))). '</div>';
    // Show the pager
    $output .= theme('pager', array('quantity',count($result_arr)));   

    return $output;
}