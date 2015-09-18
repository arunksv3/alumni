<?php 
/**
 * Career Display all parent level specializations 
 */
function career_front_subparent_specializations($eid) {   
    
  /*Decrypting the id using configration key*/
   $decrypting_key=variable_get('IIUM_Encryption_Key', NULL);
   $id=(int)decrypt($eid,$decrypting_key); 
     
   $all_specializations = db_select('specializations_p','p');
           $all_specializations->innerjoin('specializations_sp', 'sp', 'p.sid=sp.spid');
           $all_specializations->fields('p',array('name','description'));  
           $all_specializations->fields('sp',array('name','sid'));        
           $all_specializations->condition('p.sid',$id, '=');
           
           
          $display_specializations=$all_specializations->execute()
                                                       ->fetchAll();  
         //sp_name
          
   $display="";$i=1;
     foreach($display_specializations as $value): 
         if($i==1)
          $display.='<div class="careers-subparrent">'.$value->description.'<h2>Careers in '.$value->name.'</h2>'; 
          $display.='<p><strong>'.$value->sp_name.'</strong></p>'; 
          $specializations_c = db_select('specializations_c')
                               ->fields('specializations_c',array('name')) 
                               ->condition('cid',$value->sid, '=')
                               ->execute()
                               ->fetchAll();
         $display.='<ul>';  
          foreach($specializations_c as $child): 
           $display.='<li>'.$child->name.'</li>';   
          endforeach;
         $display.='</ul>';  
         $i++;
     endforeach;
     $display.='</div>';  
     
    return $display;
}
function title_function($eid) {
  /*Decrypting the id using configration key*/
   $decrypting_key=variable_get('IIUM_Encryption_Key', NULL);
   $id=(int)decrypt($eid,$decrypting_key); 
   
   $all_specializations = db_select('specializations_p')
                         ->fields('specializations_p',array('name'))
                         ->condition('sid',$id, '=')
                         ->execute()
                         ->fetchAll();

  return $all_specializations[0]->name;
}
?>