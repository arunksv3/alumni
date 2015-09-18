<?php
function events_news_update_news($form, &$form_state, $eid) {
    global $user;
    global $base_url;
    drupal_add_js(drupal_get_path('theme', 'bootstrap_business') . '/js/jquery.validate.js');
    drupal_add_js(drupal_get_path('theme', 'bootstrap_business') . '/js/jqueryformvalidation.js');
    drupal_add_js(drupal_get_path('module', 'events_news') . '/js/custom_validations.js');
    drupal_add_css(drupal_get_path('theme', 'bootstrap_business') . '/css/jquery-ui.css');
    drupal_add_js(drupal_get_path('theme', 'bootstrap_business') . '/js/jquery-ui.js');
    drupal_add_js('jQuery(document).ready(function($) {         

        $("#date").datepicker({
        changeMonth: true,
        changeYear: true,
        showOn: "both",
        yearRange: "2015:2020",
        buttonImage: "'.file_create_url('public://calendar.gif').'",
        buttonImageOnly: true,
        onSelect: function(selected) {
        }
        }); 
        
        });', 'inline');
     
    $decrypting_key=variable_get('IIUM_Encryption_Key', NULL);
    $eid=(int)decrypt($eid,$decrypting_key);
   
    $events = db_select('news')
        ->fields('news')    
        ->condition('id', $eid, '=')
        ->execute();  
    while($row = $events->fetchAssoc($events)){
        $id = $row['id']; 
        $title = $row['title']; 
        $date = $row['date']; 
        $time = $row['time']; 
        $reported_by= $row['reported_by']; 
        $category = $row['category']; 
        $photo = $row['photo'];
        if($photo!='')
            $image = ' <img src="'.file_create_url($photo).'"/>';
        else
            $image = '<img src="'.file_create_url('public://uploads/profile_pictures/noimage.jpg').'"/>';
        $description = $row['description']; 
    }
    
    $attachments = db_select('news_attachments')
        ->fields('news_attachments')    
        ->condition('news_id', $eid, '=')
        ->execute(); 
  
    $output = '';  
    while($row = $attachments->fetchAssoc($attachments)){ 
        $id = $row['id'];
        $attachment = ' <img height="100" width="100" src="'.file_create_url($row['attachment']).'"/>';
        $output.='<div id="img'.$id.'">'.$attachment.'<span><a href="javascript:void(0);" onclick="return deleteImage(\''.$base_url.'/deletenewsimage\','.$id.');">'
            . '<i id="icon-delete" class="icon-delete"></i></a></span></div>';
    } 
    
    $form['title'] = array(
        '#type' => 'item',
        '#markup' => '<h2>Update News</h2>',         
    );
   
    
    $form['news_title'] = array(
        '#id' => 'news_title',
        '#title' => 'Title<span class="form-required">*</span>',        
        '#type' => 'textfield',         
        '#default_value' =>$title,  
     ); 
 
     $form['date'] = array(
        '#id' => 'date',
        '#title' => 'Date<span class="form-required">*</span>',        
        '#type' => 'textfield',   
        '#attributes' => array('readonly'=>'readonly'),
        '#default_value' =>$date,  
     );
    
    $form['time'] = array(
        '#id' => 'from_time',
        '#title' => 'Time<span class="form-required">*</span>',        
        '#type' => 'select',
        '#options' => array(''=>'Select') + event_time(),     
        '#default_value' =>$time,  
     );
    
     $form['photo'] = array(
        '#id' => 'photo',
        '#title' => 'Upload Image',
        '#description' => 'Allowed extensions (.png, .jpeg, .gif)',
        '#type' => 'file',         
        '#default_value' =>'',  
         '#prefix' => '<div >',
        '#suffix' => $image.'</div>',
     );       
    
     $form['reported_by'] = array(
         '#id' => 'reported_by',
        '#type' => 'select',
        '#title' => t('Reported by<span class="form-required">*</span>'),
        '#options' => array(''=>'Select') + reported(),
         '#default_value' =>$reported_by,  
    );
     
     $form['category'] = array(
         '#id' => 'category',
        '#type' => 'select',
        '#title' => t('Category<span class="form-required">*</span>'),
        '#options' => array(''=>'Select') + news_category(),
         '#default_value' =>$category,  
    );
     
      $form['attachments'] = array(
        '#id' => 'attachments',
        '#title' => 'Attachments',
        '#description' => 'Allowed extensions (.png, .jpeg, .gif,pdf)',
        '#type' => 'mfw_managed_file',         
        '#default_value' =>'',  
     ); 
      $form['images'] = array(
          '#type' => 'item',
          '#markup' => $output
      );
     $form['description'] = array(
        '#id' => 'description',
        '#title' => 'Description<span class="form-required">*</span>',        
        '#type' => 'text_format',       
        '#format' => filter_default_format(),
         '#rows'=>'10', 
        '#default_value' =>$description,  
     );
     
 
     $form['eid'] = array(
            '#type' => 'hidden',
            '#value' => $eid,
        );
    $form['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Submit'),
        '#prefix' => '<div class="submit_clear_but">',
    );

    $form['cancel_skills'] = array(
        '#type' => 'item', 
        '#markup' => t('<a  class="experience_calcel" href="'.$base_url.'/'.get_admin_link().'eventsandnews/news">Cancel</a>'),
        '#suffix' => '</div>',
    );
 
 

    return $form;
}


function events_news_update_news_validate($form, &$form_state) {   
    
    // file extension validation
    $file_logo = $_FILES["files"]["name"]['photo'];
    $allowed =  array('gif','png' ,'jpg', 'jpeg');
    if($file_logo!=""){
        $logoext = pathinfo($file_logo, PATHINFO_EXTENSION);        
        if(!in_array($logoext, $allowed) ) {
            form_set_error('photo', t('Invalid file extension for profile photo'));
        }
    } 
     
}


function events_news_update_news_submit($form, &$form_state) {
    global $user;
    extract($form_state['input']);
    $description = $description['value'];
    $params = array( 
        'title' => $news_title, 
        'date' => date('Y-m-d',strtotime($date)), 
        'time' => $time, 
        'category' => $category, 
        'reported_by' => $reported_by, 
        'description' => $description, 
     );
    
    if (isset($_FILES["files"]["name"]['photo']) && $_FILES["files"]["name"]['photo'] != '') {
        $photo_name = $_FILES["files"]["name"]['photo'];
        // Move files to destination folder
        $photo_temp = $_FILES["files"]["tmp_name"]['photo'];
        $photo = time() . '_' . $photo_name;
        $dest_photo = 'public://uploads/events/' . $photo;
        move_uploaded_file($photo_temp, $dest_photo);
        $params['photo'] = $dest_photo;
    } 
    
    db_update('news')
            ->fields($params)
            ->condition('id', $eid, '=')
            ->execute();
    if (is_array($_FILES["files"]["name"]['attachments']) && sizeof($_FILES["files"]["name"]['photo']) > 0) {
        for($i=0;$i<sizeof($_FILES["files"]["name"]['attachments']);$i++) {
            $tmp_name = $_FILES["files"]["tmp_name"]['attachments'][$i];
            $name = $_FILES["files"]["name"]['attachments'][$i];
            $photo = time() . '_' . $name;
            $dest_photo = 'public://uploads/events/' . $photo;
            if(move_uploaded_file($tmp_name, $dest_photo)){
                $params = array('news_id'=>$eid,'attachment'=>$dest_photo);
                db_insert('news_attachments')
                    ->fields($params)
                    ->execute(); 
            }
        }
    }
    audit_log('News Updated', 'News ID:'.sprintf("NW-%04d", $eid));   
    drupal_set_message('News Updated.');
    drupal_goto(get_admin_link().'eventsandnews/news');
}
