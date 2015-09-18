<?php
/**
 * @file
 * A psychometric module that displays form to view section
 */
function psychometrics_view_sec($eid) { 
    $secret_key = variable_get('IIUM_Encryption_Key', NULL);
    $id = (int)decrypt($eid, $secret_key);
    
    /* query to fetch psychometric_sections */
    $sec_query = db_select('psychometric_sections')
        ->fields('psychometric_sections')
        ->condition('id', $id, '=')
        ->condition('delete_status', '0', '=')
        ->execute();
    $sec_info = $sec_query->fetchAssoc();
    
    $output .= '<ul>
                    <li><label>Title</label>'.$sec_info['title'].'</li>
                    <li><label>Description</label>'.$sec_info['description'].'</li>                    
                </ul>';
    
    return $output;
}
