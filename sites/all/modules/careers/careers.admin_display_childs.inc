<?php 
/**
  * Form builder; Display all specializations at parrent level
*/
function career_display_childs($form, &$form_state) {
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
            
          $employer_jobs_delete=db_update('specializations_c')
                         ->fields($fields)
                         ->condition('sid',$delid)
                         ->execute(); 
          
        drupal_set_message('Specialization has been deleted successfully', 'status');
        drupal_goto('admin/config/people/careers/subparrent/child');
         
     }
   
  $form['item'] = array(
      '#type' => 'item',
      '#markup' => '<ul class="action-links"><li><a href="child/add">Add specilization</a></li></ul>',
    );
  $header = array(
             array('data' => t('Title')),
             array('data' => t('Sub parrent')),
             array('data' => t('Options')),
           );
  
    $query = db_select('specializations_c', 'c');
    $query->innerjoin('specializations_sp', 'sp', 'sp.sid = c.cid');
    $query->fields('sp', array('name'));
    $query->fields('c', array('name','sid'));
    $query->condition('c.delete_status',0,'=');
    
    $table_sort = $query->extend('TableSort') // Add table sort extender.
                      ->orderByHeader($header); // Add order by headers.

    $pager = $table_sort->extend('PagerDefault')
                       ->orderBy('sid', 'DESC')
                       ->limit(10); // 10 rows per page.
     //$result = $pager->execute()->fetchAll(PDO::FETCH_ASSOC);
     //print_r($result);
     $result = $pager->execute();
     $rows = array();
     foreach($result as $res){  
      $rows[] = array($res->c_name,$res->name,'<a href="child/'.encrypt($res->sid,variable_get('IIUM_Encryption_Key', NULL)).'/edit" class="edit" alt="EDIT" title="EDIT">Edit</a> <a href="?delid='.encrypt($res->sid,variable_get('IIUM_Encryption_Key', NULL)).'" onclick="return del();" class="delete" title="DELETE" alt="DELETE">Delete</a>');  
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