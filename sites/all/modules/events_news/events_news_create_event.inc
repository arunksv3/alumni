<?php
function events_news_create_event($form, &$form_state) {
    global $user;
    global $base_url;
    drupal_add_js(drupal_get_path('theme', 'bootstrap_business') . '/js/jquery.validate.js');
    drupal_add_js(drupal_get_path('theme', 'bootstrap_business') . '/js/jqueryformvalidation.js');
    drupal_add_js(drupal_get_path('module', 'events_news') . '/js/custom_validations.js');
    drupal_add_css(drupal_get_path('theme', 'bootstrap_business') . '/css/jquery-ui.css');
    drupal_add_js(drupal_get_path('theme', 'bootstrap_business') . '/js/jquery-ui.js');
    drupal_add_js('jQuery(document).ready(function($) {         

        $("#from_date").datepicker({
        changeMonth: true,
        changeYear: true,
        showOn: "both",
        yearRange: "2015:2020",
        buttonImage: "'.file_create_url('public://calendar.gif').'",
        buttonImageOnly: true,
        onSelect: function(selected) {
        }
        });
        
        $("#to_date").datepicker({
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
        '#markup' => '<h2>Create Event</h2>',         
    );
  
     /*$form['event_code'] = array(
        '#id' => 'event_code',
        '#title' => 'Event Code<span class=" form-required">*</span>',        
        '#type' => 'textfield',         
        '#default_value' =>'',  
     );*/
    
    $form['event_title'] = array(
        '#id' => 'event_title',
        '#title' => 'Title<span class=" form-required">*</span>',        
        '#type' => 'textfield',         
        '#default_value' =>'',  
     );
    
    
    $form['date'] = array(
        '#type' => 'item',
        '#title' => 'Date',  
     );
    
    $form['date']['from_date'] = array(
        '#id' => 'from_date',
        '#title' => 'From<span class=" form-required">*</span>',        
        '#type' => 'textfield',       
        '#attributes' => array('readonly'=>'readonly'),
        '#default_value' =>'',  
     );
    
    $form['date']['from_time'] = array(
        '#id' => 'from_time',
        '#title' => 'Time<span class=" form-required">*</span>',        
        '#type' => 'select',
        '#options' => array(''=>'Select') + event_time(),     
        '#default_value' =>'',  
     );
    
     $form['date']['to_date'] = array(
        '#id' => 'to_date',
        '#title' => 'To<span class=" form-required">*</span>',        
        '#type' => 'textfield',     
         '#attributes' => array('readonly'=>'readonly'),
        '#default_value' =>'',  
     );
     $form['date']['to_time'] = array(
        '#id' => 'to_time',
        '#title' => 'Time<span class=" form-required">*</span>',        
        '#type' => 'select',
        '#options' => array(''=>'Select') + event_time(),
        '#default_value' =>'',  
     );
     
      $form['venue'] = array(
        '#type' => 'item',
        '#title' => 'Venue',  
     );
    
    $form['venue']['location'] = array(
        '#id' => 'location',
        '#title' => 'Location<span class=" form-required">*</span>',        
        '#type' => 'textfield',         
        '#default_value' =>'',  
     );
     
    $form['venue']['country'] = array(
         '#id' => 'country',
        '#type' => 'select',
        '#title' => t('Country<span class=" form-required">*</span>'),
        '#options' => array(''=>'Select') + countries_list(),
        '#ajax' => array(
            'event' => 'change',
            'callback' => 'event_state_list_ajax',
            'wrapper' => 'event-state-list-wrapper',
        ),
        '#default_value' => (isset($country) ? $country : ''),
    );
    
    // Wrapper for state dropdown list
    $form['venue']['wrapper'] = array(
        '#prefix' => '<div id="event-state-list-wrapper">',
        '#suffix' => '</div>',
    );
    
    if ((isset($form_state['values']['country']) && $form_state['values']['country'] != "")) {
        $cid = $form_state['values']['country'];
        $state_list = db_select('states_list')
                ->fields('states_list')
                ->condition('cid', $cid, '=')
                ->execute()
                ->fetchAll();
        $state_list_arr[''] = 'Select';
        foreach ($state_list as $value):
            $state_list_arr[$value->id] = $value->name;
        endforeach;
        if(count($state_list) > 0){
            // Province dropdown list
            $form['venue']['wrapper']['state'] = array(
                '#id' => 'state',
                '#type' => 'select',
                '#title' => t('State<span class=" form-required">*</span>'),
                '#multiple' => false,
                '#options' => $state_list_arr,
                '#default_value' => '',                
            );
        }
    }
            
     $form['venue']['photo'] = array(
        '#id' => 'photo',
        '#title' => 'Upload Image',
        '#description' => 'Allowed extensions (.png, .jpeg, .gif)',
        '#type' => 'file',       
         //'#required' => TRUE,
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
     $form['venue']['note'] = array(
        '#id' => 'note',
        '#title' => 'Note<span class=" form-required">*</span>',        
        '#type' => 'textfield',         
        '#default_value' =>'',  
    );
     
      $form['organized_by'] = array(
         '#id' => 'organized_by',
        '#type' => 'select',
        '#title' => t('Organized by<span class=" form-required">*</span>'),
        '#options' => array(''=>'Select') + organized(),
    );
     
      $form['attachments'] = array(
        '#id' => 'attachments',
        '#title' => 'Attachments',
        '#description' => 'Allowed extensions (.png, .jpeg, .gif,pdf)',
        '#type' => 'mfw_managed_file',         
        '#default_value' =>'',  
     ); 
      
     $form['registration'] = array(
            '#id' => 'registration',
            '#type' => 'radios',
            '#title' => t('Is this event have student registration?'), 
            '#options' => array(
                1 => t('Yes'), 
                0 => t('No')
                ),
            //'#required' => TRUE,
            '#default_value' => 0,
            
        );
 
     
    $form['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Submit'),
        '#prefix' => '<div class="submit_clear_but">',
    );

    $form['cancel_skills'] = array(
        '#type' => 'item', 
        '#markup' => t('<a  class="experience_calcel" href="'.$base_url.'/'.get_admin_link().'eventsandnews/events">Cancel</a>'),
        '#suffix' => '</div>',
    );
 
    $form['#attributes'] = array('id'=>'events-news-create-event');

    return $form;
}


function events_news_create_event_validate($form, &$form_state) {   
    
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


function events_news_create_event_submit($form, &$form_state) {
    global $user;
    global $base_url;
    extract($form_state['input']);
    $description = $description['value'];
    $params = array(
        //'event_code' => $event_code,
        'title' => $event_title,
        'from_date' => date('Y-m-d',strtotime($from_date)),
        'from_time' => $from_time,
        'to_date' => date('Y-m-d',strtotime($to_date)),
        'to_time' => $to_time,
        'location' => $location,
        'country' => $country,        
        'description' => $description,
        'note' => $note,
        'registration_required' => $registration,
        'organized_by' => $organized_by,
        'uid' => $user->uid,
        'created_date' => date('Y-m-d H:i:s')
    );
    if(isset($state) && $state>0){
        $params['state'] = $state;
    }
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
    $event_id = db_insert('events')
            ->fields($params)
            ->execute();
 
    
    if (is_array($_FILES["files"]["name"]['attachments']) && sizeof($_FILES["files"]["name"]['photo']) > 0) {
        for($i=0;$i<sizeof($_FILES["files"]["name"]['attachments']);$i++) {
            $tmp_name = $_FILES["files"]["tmp_name"]['attachments'][$i];
            $name = $_FILES["files"]["name"]['attachments'][$i];
            $photo = time() . '_' . $name;
            $dest_photo = 'public://uploads/events/' . $photo;
            if(move_uploaded_file($tmp_name, $dest_photo)){
                $params = array('event_id'=>$event_id,'attachment'=>$dest_photo);
                db_insert('event_attachments')
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
                    <td>The Event '.sprintf("EV-%04d", $event_id).' is waiting for your approval.</td>
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
        $subject = "Event is waiting for approval"; 
        events_news_mail($from, $to, $subject, $message);
     audit_log('Event Created', 'Event ID:'.sprintf("EV-%04d", $event_id));   
    drupal_set_message('Event Created.');
    drupal_goto(get_admin_link().'eventsandnews/events');
}
