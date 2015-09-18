<?php
/**
 * @file
 * A psychometric module that displays form to view scale
 */
function psychometrics_view_scale($eid) { 
    $secret_key = variable_get('IIUM_Encryption_Key', NULL);
    $id = (int)decrypt($eid, $secret_key);
    
    /* query to fetch psychometric_sections */
    $sec_query = db_select('psychometric_scales')
        ->fields('psychometric_scales')
        ->condition('id', $id, '=')
        ->condition('delete_status', '0', '=')
        ->execute();
    $sec_info = $sec_query->fetchAssoc();
    
    $output .= '<ul>
                    <li><label>Section Title</label>'.get_section_name($sec_info['secid']).'</li>
                    <li><label>Domain Title</label>'.$sec_info['title'].'</li>  
                </ul>';
    
    return $output;
}
