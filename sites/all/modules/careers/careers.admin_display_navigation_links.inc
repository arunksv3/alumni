<?php 
/**
 * Display all specialization links
 */
function career_display_all_navigation_links() {
    $links='<ul><li><a href="careers/display">Parent Specializations</a></li><li><a href="careers/subparrent">Sub Parent Specializations</a></li>'
            . '<li><a href="careers/subparrent/child">Sub Parent child Specializations</a></li></ul>';
   return $links;
}
?>