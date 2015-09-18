<?php
/**
 * @file
 * A psychometric module that displays form to view section
 */
function psychometrics_view_dom($eid) { 
    $secret_key = variable_get('IIUM_Encryption_Key', NULL);
    $id = (int)decrypt($eid, $secret_key);
    
    /* query to fetch psychometric_sections */
    $sec_query = db_select('psychometric_domains')
        ->fields('psychometric_domains')
        ->condition('id', $id, '=')
        ->condition('delete_status', '0', '=')
        ->execute();
    $sec_info = $sec_query->fetchAssoc();
    
    $output .= '<ul>
                    <li><label>Section Title</label>'.get_section_name($sec_info['secid']).'</li>
                    <li><label>Domain Title</label>'.$sec_info['title'].'</li>  
                    <li><label>Description</label>'.$sec_info['desc'].'</li>
                    <li><label>Low Meanings</label>'.$sec_info['low_meanings'].'</li>  
                    <li><label>Moderate</label>'.$sec_info['moderate'].'</li>
                    <li><label>High Meanings</label>'.$sec_info['high_meanings'].'</li>  
                </ul>';
    
    return $output;
}
