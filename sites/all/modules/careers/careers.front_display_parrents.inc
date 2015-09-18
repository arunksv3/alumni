<?php 
/**
 * Career Display all parent level specializations 
 */
function career_front_parent_specializations() {     
  $specializations_p = db_select('specializations_p')
                               ->fields('specializations_p',array('sid','name','image','image_path')) 
                               ->condition('delete_status',0, '=')
                               ->execute()
                               ->fetchAll();  
  
   $display="";
   //Fetch the content from drupal carrer page in admin section
   $nid = 3;
   $node = node_load($nid);
   if($node!="")
   {
     $nodeView = node_view($node, 'full');
     $display.= '<div class="career">'.drupal_render($nodeView['body']);
   }
  
  $display.='<ul class="carrers-list">';
    foreach($specializations_p as $value): 
     $variables = array(
                'path' => $value->image_path, 
                'alt' => $value->image,
                'title' => $value->image,
                'width' => '265',
                'height' => '175',
            );   
     $display.='<li><a href="careers/'.encrypt($value->sid,variable_get('IIUM_Encryption_Key', NULL)).'">'.theme('image', $variables).'<br><h2>'.$value->name.'</h2></a></li>'; 
    endforeach;
 $display.='</ul></div>';

    return $display;
}
?>