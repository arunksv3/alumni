<?php 
/**
 * Career specializations insert code
 */
function career_add_form_submit($form, &$form_state) {
    
    extract($form_state['input']);
    //print_r($form_state) ;exit;
    /*Image upload code start*/
     $filename = $_FILES["icon"]["name"]; 
     if($filename!="")
     {
      $image_path=variable_get('IIUM_career_image_path', NULL);
      $icon_temp = $_FILES["icon"]["tmp_name"];        
      $dest_icon = $image_path.time().'_'.$filename;  
      move_uploaded_file($icon_temp, $dest_icon);
     }
     /*Image upload code end*/
     
         
  $specializations_p = db_insert('specializations_p')
  ->fields(array(
    'name' => trim($form_state['values']['title']),
    'image' => trim($filename),
    'image_path	' => trim($dest_icon),
    'description'=> trim($description['value']),
  ))
  ->execute();
  audit_log('parrent specialization created', 'specialization id:'.$specializations_p);
  if($specializations_p>0)
  {
   drupal_set_message('Specialization has been added successfully', 'status');
   drupal_goto('admin/config/people/careers');
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
  $specializations_p = db_update('specializations_p')
                       ->fields($fields)
                       ->condition('sid',(int)$eid)
                       ->execute();
  audit_log('parrent specialization updated', 'specialization id:'.$eid);
  if($specializations_p>0)
  {
   drupal_set_message('Specialization has been updated successfully', 'status');
   drupal_goto('admin/config/people/careers');
  }
    
}
/**
 * Career form image validation code
 */
function career_add_parrent_validate($form, &$form_state) {
    $icon = $_FILES["icon"]["name"];
    $allowed =  array('gif','png' ,'jpg', 'jpeg');
    if($form_state['clicked_button']['#attributes']['eid']=="")
    {
     if($icon=='')
      form_set_error('icon', t('Icon/Image field is required.'));  
     else if($icon!=""){
        $icon_ext = pathinfo($icon, PATHINFO_EXTENSION);        
        if(!in_array($icon_ext, $allowed) ) {
            form_set_error('icon', t('Invalid file extension for Icons/Images'));
        }
     }
    }
}
/**
  * Form builder; Create specializations in parrent level
*/
function career_add_parrent($form, &$form_state,$eid) {
    
   /*Call edit specilization sql function*/
     $edit_specilization_details=edit_specilization_sql($eid);
    
     $image_disp='';
     $submit=array('career_add_form_submit');
     $attributes=array('eid'=>'');
    if($eid!="")
    {
     $variables = array(
                  'path' => $edit_specilization_details[0]->image_path, 
                  'alt' => $edit_specilization_details[0]->image,
                  'title' => $edit_specilization_details[0]->image,
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
      '#title' => 'Icon/Image <span title="This field is required." class="form-required">*</span>',  
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
      $specilization_detail = db_select('specializations_p','p');
      $specilization_detail->fields('p',array('name','image','image_path','description'));
      $specilization_detail->condition('p.sid',trim($eid), '='); 

      return $specilization_details= $specilization_detail->execute()
                       ->fetchAll();  
    }
    else
     return 0;   
}
?>