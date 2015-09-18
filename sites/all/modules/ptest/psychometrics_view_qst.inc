<?php
/**
 * @file
 * A psychometric module that displays form to view question
 */
function psychometrics_view_qst($eid) { 
    $secret_key = variable_get('IIUM_Encryption_Key', NULL);
    $id = (int)decrypt($eid, $secret_key);
    
    /* query to fetch psychometric_questions */
    $qst_query = db_select('psychometric_questions')
        ->fields('psychometric_questions')
        ->condition('id', $id, '=')
        ->condition('delete_status', '0', '=')
        ->execute();
    $qst_info = $qst_query->fetchAssoc();
    
    $output .= '<ul>
                    <li><label>Section Title</label>'.get_section_name($qst_info['secid']).'</li>
                    <li><label>Domain Title</label>'.get_domain_name($qst_info['domid']).'</li>  
                    <li><label>Question</label>'.$qst_info['question'].'</li>
                    <li><label>Reverse Score</label>'.($qst_info['reverse_score']=='0'?'Yes':'No').'</li>  
                </ul>';
    
    return $output;
}
