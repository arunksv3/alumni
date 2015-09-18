<?php
function events_news_create_news($form, &$form_state) {
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
     
    $form['title'] = array(
        '#type' => 'item',
        '#markup' => '<h2>Create News</h2>',         
    );
    
    
    $form['news_title'] = array(
        '#id' => 'news_title',
        '#title' => 'Title<span class=" form-required">*</span>',        
        '#type' => 'textfield',         
        '#default_value' =>'',  
     );
     
     $form['date'] = array(
        '#id' => 'date',
        '#title' => 'Date<span class=" form-required">*</span>',        
        '#type' => 'textfield',   
        '#attributes' => array('readonly'=>'readonly'),
        '#default_value' =>'',  
     );
    
    $form['time'] = array(
        '#id' => 'from_time',
        '#title' => 'Time<span class=" form-required">*</span>',        
        '#type' => 'select',
        '#options' => array(''=>'Select') + event_time(),     
        '#default_value' =>'',  
     );
    
     $form['photo'] = array(
        '#id' => 'photo',
        '#title' => 'Upload Image',
        '#description' => 'Allowed extensions (.png, .jpeg, .gif)',
        '#type' => 'file',   
        // '#required' => TRUE,
        '#default_value' =>'',  
     );       
     
     $form['reported_by'] = array(
         '#id' => 'reported_by',
        '#type' => 'select',
        '#title' => t('Reported by<span class=" form-required">*</span>'),
        '#options' => array(''=>'Select') + reported(),
    );
     
     $form['category'] = array(
         '#id' => 'category',
        '#type' => 'select',
        '#title' => t('Category<span class=" form-required">*</span>'),
        '#options' => array(''=>'Select') + news_category(),
    );
     
      $form['attachments'] = array(
        '#id' => 'attachments',
        '#title' => 'Attachments',
        '#description' => 'Allowed extensions (.png, .jpeg, .gif,pdf)',
        '#type' => 'mfw_managed_file',         
        '#default_value' =>'',  
     ); 
    
     $form['description'] = array(
        '#id' => 'description',
        '#title' => 'Description',        
        '#type' => 'text_format',       
        '#format' => filter_default_format(),
         '#rows'=>'10',  
          '#required' => TRUE
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
 
     $form['#attributes'] = array('id'=>'events-news-update-news');

    return $form;
}
 
function events_news_create_news_validate($form, &$form_state) {   
    
    // file extension validation
    $file_logo = $_FILES["files"]["name"]['photo'];
    $allowed =  array('gif','png' ,'jpg', 'jpeg');
    if($file_logo!=""){
        $logoext = pathinfo($file_logo, PATHINFO_EXTENSION);        
        if(!in_array($logoext, $allowed) ) {
            form_set_error('photo', t('Invalid file extension for profile photo'));
        }
    }  
//    else
//        form_set_error('photo', t('Please upload image'));
     
}

function events_news_create_news_submit($form, &$form_state) {
    global $user;
    global $base_url;
    extract($form_state['input']);
    $description = $description['value'];
    $params = array( 
        'title' => $news_title, 
        'date' => date('Y-m-d',strtotime($date)), 
        'time' => $time, 
        'category' => $category, 
        'reported_by' => $reported_by, 
        'description' => $description, 
        'uid' => $user->uid,
        'created_date' => date('Y-m-d H:i:s')
    );
   
    if (isset($_FILES["files"]["name"]['photo']) && $_FILES["files"]["name"]['photo'] != '') {
        $photo_name = $_FILES["files"]["name"]['photo'];
        // Move files to destination folder
        $photo_temp = $_FILES["files"]["tmp_name"]['photo'];
        $photo = time() . '_' . $photo_name;
        $dest_photo = 'public://uploads/events/' . $photo;
        move_uploaded_file($photo_temp, $dest_photo);
    } else {
        $dest_photo = '';
    }
    $params['photo'] = $dest_photo;
    $news_id = db_insert('news')
            ->fields($params)
            ->execute();
    
    if (is_array($_FILES["files"]["name"]['attachments']) && sizeof($_FILES["files"]["name"]['photo']) > 0) {
        for($i=0;$i<sizeof($_FILES["files"]["name"]['attachments']);$i++) {
            $tmp_name = $_FILES["files"]["tmp_name"]['attachments'][$i];
            $name = $_FILES["files"]["name"]['attachments'][$i];
            $photo = time() . '_' . $name;
            $dest_photo = 'public://uploads/events/' . $photo;
            if(move_uploaded_file($tmp_name, $dest_photo)){
                $params = array('news_id'=>$news_id,'attachment'=>$dest_photo);
                db_insert('news_attachments')
                    ->fields($params)
                    ->execute(); 
            }
        }
    }
    
    $path = $base_url.'/'.drupal_get_path('theme', 'bootstrap_business'); 
    $message = '<html xmlns="http://www.w3.org/1999/xhtml">
        <head>
            <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
            <title>IIUM Email</title>
        </head>
        <body style="font-family:Arial, Helvetica, sans-serif; font-size:14px; line-height:30px; margin:0; padding:0;">
            <table width="600" border="0" cellspacing="0" cellpadding="0" style="margin:0 auto; border: 1px solid #ccc; padding:0 0 15px 0;">
                <tr>
                  <td style="background:url(' . $path . '/images/header-bg.jpg) repeat-x left top;border-bottom: 5px solid #9c2b3d; padding: 15px 10px; text-align: center;"><img src="' . $path . '/images/logo.png" width="228" height="83" /></td>
                </tr>
                <tr>
                    <td>The News '.sprintf("NW-%04d", $news_id).' is waiting for your approval.</td>
                </tr>
                <tr>
                    <td>Regards, </br>
                    IIUM Career Consuling Team</td>
                </tr> 
            </table>    
        </body>
        </html>'; 
        $to = variable_get('site_mail');
        $from = 'info@alumni-career.iium.edu.my';
        $subject = "News is waiting for approval"; 
        events_news_mail($from, $to, $subject, $message);
        audit_log('News Created', 'News ID:'.sprintf("NW-%04d", $news_id));   
    drupal_set_message('News Created.');
    drupal_goto(get_admin_link().'eventsandnews/news');
}
