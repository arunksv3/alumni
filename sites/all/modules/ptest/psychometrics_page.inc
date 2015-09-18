<?php
/**
 * @file
 * A psychometric module that displays page content.
 */
function psychometrics_page() {      
    $nid = 6;
    global $user;
    $node = node_load($nid);  
    $std_role_info = user_role_load_by_name('student');
    if($node!=""){
        $nodeView = node_view($node, 'full');        
        $output = '<div class="psychometrics-content">'.drupal_render($nodeView['body']);
        $output .= render($nodeView['field_pageimage']).'</div>';
    }     
    
    if ($user->uid > 0 && in_array($std_role_info->name, $user->roles)) { 
        $output .= '<div class="take-the-test">
                        '.l('Take The Test', "student/psychometrics/demographic_information/rNbTrMQTeYLWk8inNYu1PQ").'                    
                    </div>';
    }
    
    return $output;
}
