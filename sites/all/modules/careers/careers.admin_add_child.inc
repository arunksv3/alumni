<?php 
/**
 * Career specializations insert code
 */
function career_add_form_submit($form, &$form_state) {
    
    extract($form_state['input']);
    //print_r($form_state) ;exit;
    /*Image upload code start*/
     $filename = $_FILES["icon"]["name"]; 
     $dest_icon="";
     if($filename!="")
     {
      $image_path=variable_get('IIUM_career_image_path', NULL);
      $icon_temp = $_FILES["icon"]["tmp_name"];        
      $dest_icon = $image_path.time().'_'.$filename;  
      move_uploaded_file($icon_temp, $dest_icon);
     }
     /*Image upload code end*/
     
         
  $specializations_c = db_insert('specializations_c')
  ->fields(array(
    'pid' => trim($form_state['values']['specialization_parrent']),
    'cid' => trim($form_state['values']['specialization_subparrent']),
    'name' => trim($form_state['values']['title']),
    'image' => trim($filename),
    'image_path	' => trim($dest_icon),
    'description'=> trim($description['value']),
  ))
  ->execute();
    audit_log('child specialization created', 'specialization id:'.$specializations_c);

  if($specializations_c>0)
  {
   drupal_set_message('Specialization has been added successfully', 'status');
   drupal_goto('admin/config/people/careers/subparrent/child');
  }
    
}
/**
 * Career specializations update code
 */
function career_edit_form_submit($form,&$form_state) {
        extract($form_state['input']);
    /*Decrypting the id using configration key*/
      $decrypting_key=variable_get('IIUM_Encryption_Key', NULL);
      $eid=decrypt($form_state['clicked_button']['#attributes']['eid'],$decrypting_key);

    /*Image upload code start*/
     $filename = $_FILES["icon"]["name"]; 
     $fields=array(
        'pid' => trim($form_state['values']['specialization_parrent']),
        'cid'=> trim($form_state['values']['specialization_subparrent']),
        'name' => trim($form_state['values']['title']),
        'description'=> trim($description['value']),
      );  
     if($filename!="")
     {
      $image_path=variable_get('IIUM_career_image_path', NULL);
      $icon_temp = $_FILES["icon"]["tmp_name"];        
      $dest_icon = $image_path.time().'_'.$filename;  
      if(file_exists($form_state['values']['image_path']))
       unlink(trim($form_state['values']['image_path']));
      move_uploaded_file($icon_temp, $dest_icon);
       $fields=$fields+array('image' => trim($filename),
        'image_path' => trim($dest_icon),);
     }
     /*Image upload code end*/  
     
    //update query for specializations 
  $specializations_p = db_update('specializations_c')
                       ->fields($fields)
                       ->condition('sid',(int)$eid)
                       ->execute();
 audit_log('child specialization updated', 'specialization id:'.$eid);
 
   drupal_set_message('Specialization has been updated successfully', 'status');
   drupal_goto('admin/config/people/careers/subparrent/child');
  
    
}
/**
 * Career form image validation code
 */
function career_add_child_validate($form, &$form_state) {
    $icon = $_FILES["icon"]["name"];
    $allowed =  array('gif','png' ,'jpg', 'jpeg');
      if($icon!=""){
        $icon_ext = pathinfo($icon, PATHINFO_EXTENSION);        
        if(!in_array($icon_ext, $allowed) ) {
            form_set_error('icon', t('Invalid file extension for Icons/Images'));
        }
     }
    
}
/**
  * Form builder; Create specializations in sub parrent level
*/
function career_add_child($form, &$form_state,$eid) {
    
   /*Call edit specilization sql function*/
     $edit_specilization_details=edit_specilization_sql($eid);
     $image_disp='';
     $submit=array('career_add_form_submit');
     $attributes=array();
    if($eid!="")
    {
     $variables = array(
                  'path' => $edit_specilization_details[0]->image_path, 
                  'alt' => $edit_specilization_details[0]->image_path,
                  'title' => $edit_specilization_details[0]->image_path,
                  'width' => '150',
                  'height' => '150',
                  'attributes' => array('class' => 'some-img'),
              );
      $form['icon_image_path'] = array(
      '#type' => 'hidden', 
      '#value' => $edit_specilization_details[0]->image_path,
     );
      if($edit_specilization_details[0]->image_path!="")
       $image_disp=theme('image', $variables);
      
     $submit=array('career_edit_form_submit');
     $attributes= array('eid'=>$eid);
   }
    
  
  $form['filter'] = array(
      '#type' => 'fieldset',
    );
  $form['filter']['specialization_parrent'] = array(
      '#type' => 'select',
      '#name' => 'specialization_parrent',
      '#title' => 'Specialization',
      '#required' => TRUE, //make this field required
      '#options' => specialization_parrent_carrer(),
      '#default_value' => (isset($edit_specilization_details[0]->pid)?$edit_specilization_details[0]->pid:''),
      '#ajax' => array(
       'event'=>'change',
       'callback' => 'employer_subparrent_list_disp_ajax',
       'wrapper' => 'subparrent-list-wrapper',
     ),
    );
   // Wrapper for sub parent dropdown list
    $form['filter']['wrapper'] = array(
     '#prefix' => '<div id="subparrent-list-wrapper">',
     '#suffix' => '</div>',
    );
    if (isset($form_state['values']['specialization_parrent'])&& $form_state['values']['specialization_parrent']!="" || $eid!="") {
        if($eid!="" && !isset($form_state['values']['specialization_parrent']))
          $specialization_parrent=$edit_specilization_details[0]->pid;  
        else 
          $specialization_parrent=$form_state['values']['specialization_parrent'];  
      
        $category_subparrents = db_select('specializations_sp')
                           ->fields('specializations_sp') 
                           ->condition('spid',$specialization_parrent,'=')   
                           ->execute()
                           ->fetchAll(); 
        $display_category_subparrents['']= 'Select';   
        foreach($category_subparrents as $category_subparrent_name): 
          $display_category_subparrents[$category_subparrent_name->sid]= $category_subparrent_name->name;   
        endforeach;
         // Province dropdown list
     $form['filter']['wrapper']['specialization_subparrent'] = array(
      '#type' => 'select',
      '#name' => 'specialization_subparrent',
      '#title' => 'Specialization',
       '#required' => TRUE, //make this field required
      '#options' => $display_category_subparrents,
      '#default_value' => (isset($edit_specilization_details[0]->cid)?$edit_specilization_details[0]->cid:''),
     );         
    }
  $form['filter']['title'] = array(
      '#type' => 'textfield',
      '#name' => 'title',
      '#title' => 'Title',
      '#required' => TRUE, //make this field required
      '#maxlength' => 60, 
      '#default_value' => (isset($edit_specilization_details[0]->name)?$edit_specilization_details[0]->name:'')
    );
  
  $form['filter']['icon'] = array(
      '#type' => 'file',
      '#name' => 'icon',
      '#description' => t('Click "Browse..." to select an image to upload.'),
      '#title' => 'Icon/Image',  
      '#prefix' => '<div class="form-item form-type-file form-item-files-company-logo">',
      '#suffix' =>$image_disp.'</div>',
    );
  $form['filter']['description'] = array(
      '#type' => 'text_format',
      '#format' => filter_default_format(),
      '#name' => 'description',
      '#title' => 'Description',       
      '#required' => TRUE,     
      '#rows'=>'10',
      '#default_value' => (isset($edit_specilization_details[0]->description)?$edit_specilization_details[0]->description:'')
    );
   $form['filter']['image_path'] = array(
      '#type' => 'hidden',
      '#value' =>(isset($edit_specilization_details[0]->image_path)?$edit_specilization_details[0]->image_path:''),
    );
  // Submit button
  $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('submit'),
      '#submit' => $submit,
      '#attributes'=>$attributes,
    );

 return $form;
}
/*Get subparrent values threw AJAX function*/
function employer_subparrent_list_disp_ajax($form, $form_state) {
  // Return the dropdown list including the wrapper
  return $form['filter']['wrapper'];
}
/*Get perticular specialization record*/
function edit_specilization_sql($eid)
{
    
    if($eid!="")
    {
     /*Decrypting the id using configration key*/
      $decrypting_key=variable_get('IIUM_Encryption_Key', NULL);
      $eid=decrypt($eid,$decrypting_key);
      
      if($eid==0)
      {
        drupal_not_found();
        drupal_exit();
      }
      $specilization_detail = db_select('specializations_c','c');
      $specilization_detail->fields('c',array('pid','cid','name','image_path','description'));
      $specilization_detail->condition('c.sid',trim($eid), '='); 

      return $specilization_details= $specilization_detail->execute()
                       ->fetchAll();  
    }
    else
     return 0;   
}
/*Get parrent level specializations*/
function specialization_parrent_carrer()
{
      $specilizations = db_select('specializations_p','p');
      $specilizations->fields('p',array('name','sid'));
      $specilizations_records= $specilizations->execute()
                                     ->fetchAll();  
      $specilization_select['']= 'Select Specialization'; 
      foreach($specilizations_records as $specilizations_record):
       $specilization_select[$specilizations_record->sid]= $specilizations_record->name; 
      endforeach;
      
      return $specilization_select;
}
?>