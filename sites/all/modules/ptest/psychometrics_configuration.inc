<?php
/**
 * @file
 * A psychometric module that displays form to attend for test.
 */
function psychometrics_configuration($form, &$form_state) {   
    $links = '<ul>
                <li>'.l('Sections','').'</li>
                <li>'.l('Domains','').'</li>
                <li>'.l('Scales','').'</li>
                <li>'.l('Student Information','').'</li>                
            </ul>';
    
    $form['links'] = array(
        '#type' => 'item',
        '#markup' => $links,
    );
    
    return $form;
    
}