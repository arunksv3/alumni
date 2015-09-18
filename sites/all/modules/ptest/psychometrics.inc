<?php
/**
 * @file
 * A psychometric module that displays form to attend for test.
 */
function psychometrics($form, &$form_state) {   
    
    $form['sections'] = array(
        '#type' => 'item',
        '#markup' => get_sections_links(),
    );
    
    return $form;
}