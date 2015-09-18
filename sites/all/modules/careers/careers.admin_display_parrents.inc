<?php 
/**
  * Form builder; Display all specializations at parrent level
*/
function career_display_parrents($form, &$form_state) {
  drupal_add_js("function del(){
        var cnf = confirm('Are you sure you want to delete this Specialization? Click Ok to delete.');
        if(cnf)
            return true;
        else
            return false;    
         }",'inline'); 
  if(isset($_GET) && isset($_GET['delid']) && $_GET['delid']!=""){
          $fields = array(
            'delete_status' => 1,
           );
          /*Decrypting the data using configration key*/
            $decrypting_key=variable_get('IIUM_Encryption_Key', NULL);
            $delid=(int)decrypt($_GET['delid'],$decrypting_key);
            
          $employer_jobs_delete=db_update('specializations_p')
                         ->fields($fields)
                         ->condition('sid',$delid)
                         ->execute(); 
          
        drupal_set_message('Specialization has been deleted successfully', 'status');
        drupal_goto('admin/config/people/careers');
         
     }
   
  $form['item'] = array(
      '#type' => 'item',
      '#markup' => '<ul class="action-links"><li><a href="careers/add">Add specilization</a></li></ul>',
    );
  $header = array(
             array('data' => t('Title')),
             array('data' => t('Icon')),
             array('data' => t('Options')),
           );
  
    $query = db_select('specializations_p', 'p');
    $query->fields('p', array('sid','name','image','image_path'));
    $query->condition('delete_status',0,'=');
    
    $table_sort = $query->extend('TableSort') // Add table sort extender.
                      ->orderByHeader($header); // Add order by headers.

    $pager = $table_sort->extend('PagerDefault')
                       ->orderBy('sid', 'DESC')
                       ->limit(10); // 10 rows per page.

     $result = $pager->execute();
     $rows = array();
     foreach($result as $res){
        $image="";
         if($res->image_path!="")
         {
          $variables = array(
                'path' => $res->image_path, 
                'alt' => $res->image,
                'title' => $res->image,
                'width' => '50',
                'height' => '50',
                'attributes' => array('class' => 'some-img'),
            );
          $image=theme('image', $variables);
         }
      $rows[] = array($res->name,$image,'<a href="careers/'.encrypt($res->sid,variable_get('IIUM_Encryption_Key', NULL)).'/edit" class="edit" alt="EDIT" title="EDIT">Edit</a> <a href="?delid='.encrypt($res->sid,variable_get('IIUM_Encryption_Key', NULL)).'" onclick="return del();" class="delete" title="DELETE" alt="DELETE">Delete</a>');
      
    }
    
    $form['table'] = array(
      '#theme' => 'table',
      '#header' => $header,
      '#rows' => $rows,
      '#empty' => t('No Records Found!')
    );
 $form['pager'] = array('#markup' => theme('pager'));

 return $form;
}
?>