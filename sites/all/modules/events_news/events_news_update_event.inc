<?php
function events_news_update_event($form, &$form_state, $eid) {
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
     
    $decrypting_key=variable_get('IIUM_Encryption_Key', NULL);
    $eid=(int)decrypt($eid,$decrypting_key);
   
    $events = db_select('events')
        ->fields('events')    
        ->condition('id', $eid, '=')
        ->execute();  
    while($row = $events->fetchAssoc($events)){
        $id = $row['id'];
       // $event_code = $row['event_code'];
        $title = $row['title'];
        $from_date = date('m/d/Y',strtotime($row['from_date']));
        $from_time = $row['from_time'];
        $to_date = date('m/d/Y',strtotime($row['to_date']));
        $to_time = $row['to_time'];
        $location = $row['location'];
        $country = $row['country'];
        $state = $row['state'];
        $organized_by = $row['organized_by'];
        $photo = $row['photo'];
        if($photo!='')
            $image = ' <img src="'.file_create_url($photo).'"/>';
        else
            $image = '<img src="'.file_create_url('public://uploads/profile_pictures/noimage.jpg').'"/>';
        $description = $row['description'];
        $note = $row['note'];
        $registration = $row['registration_required'];
    }
    
    $attachments = db_select('event_attachments')
        ->fields('event_attachments')    
        ->condition('event_id', $eid, '=')
        ->execute(); 
      $output = '';  
    while($row = $attachments->fetchAssoc($attachments)){ 
        $id = $row['id'];
        $attachment = ' <img height="100" width="100" src="'.file_create_url($row['attachment']).'"/>';
        $output.='<div id="img'.$id.'">'.$attachment.'<span><a href="javascript:void(0);" onclick="return deleteImage(\''.$base_url.'/deleteeventimage\','.$id.');">'
            . '<i id="icon-delete" class="icon-delete"></i></a></span></div>';
    } 
    
    $form['title'] = array(
        '#type' => 'item',
        '#markup' => '<h2>Update Event</h2>',         
    );
  
   /*  $form['event_code'] = array(
        '#id' => 'event_code',
        '#title' => 'Event Code<span class="form-required">*</span>',        
        '#type' => 'textfield',         
        '#default_value' =>$event_code,  
     );*/
    
    $form['event_title'] = array(
        '#id' => 'event_title',
        '#title' => 'Title<span class="form-required">*</span>',        
        '#type' => 'textfield',         
        '#default_value' =>$title,  
     );
    
    
    $form['date'] = array(
        '#type' => 'item',
        '#title' => 'Date',  
     );
    
    $form['date']['from_date'] = array(
        '#id' => 'from_date',
        '#title' => 'From<span class="form-required">*</span>',        
        '#type' => 'textfield',  
        '#attributes' => array('readonly'=>'readonly'),
        '#default_value' =>$from_date,  
     );
    
    $form['date']['from_time'] = array(
        '#id' => 'from_time',
        '#title' => 'Time<span class="form-required">*</span>',        
        '#type' => 'select',
        '#options' => array(''=>'Select') + event_time(),     
        '#default_value' =>$from_time,  
     );
    
     $form['date']['to_date'] = array(
        '#id' => 'to_date',
        '#title' => 'To<span class="form-required">*</span>',        
        '#type' => 'textfield',   
         '#attributes' => array('readonly'=>'readonly'),
        '#default_value' =>$to_date,  
     );
     $form['date']['to_time'] = array(
        '#id' => 'to_time',
        '#title' => 'Time<span class="form-required">*</span>',        
        '#type' => 'select',
        '#options' => array(''=>'Select') + event_time(),
        '#default_value' =>$to_time,  
     );
     
      $form['venue'] = array(
        '#type' => 'item',
        '#title' => 'Venue',  
     );
    
    $form['venue']['location'] = array(
        '#id' => 'location',
        '#title' => 'Location<span class="form-required">*</span>',        
        '#type' => 'textfield',         
        '#default_value' =>$location,  
     );
     
    $form['venue']['country'] = array(
         '#id' => 'country',
        '#type' => 'select',
        '#title' => t('Country<span class="form-required">*</span>'),
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
    
    if ((isset($form_state['values']['country']) && $form_state['values']['country'] != "") || $country>0) {
        $cid = isset($form_state['values']['country'])?$form_state['values']['country']:$country;
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
                '#title' => t('State<span class="form-required">*</span>'),
                '#multiple' => false,
                '#options' => $state_list_arr,
                '#default_value' => $state,                
            );
        }
    }
            
     $form['venue']['photo'] = array(
        '#id' => 'photo',
        '#title' => 'Upload Image',
        '#description' => 'Allowed extensions (.png, .jpeg, .gif)',
        '#type' => 'file',         
        '#default_value' =>'',  
        '#prefix' => '<div >',
        '#suffix' => $image.'</div>',
     );       
    
     $form['description'] = array(
        '#id' => 'description',
        '#title' => 'Description<span class="form-required">*</span>',        
        '#type' => 'text_format',       
        '#format' => filter_default_format(),
         '#rows'=>'10', 
        '#default_value' =>$description,  
     );
     $form['venue']['note'] = array(
        '#id' => 'note',
        '#title' => 'Note<span class="form-required">*</span>',        
        '#type' => 'textfield',         
        '#default_value' =>$note,  
    );
     
      $form['organized_by'] = array(
         '#id' => 'organized_by',
        '#type' => 'select',
        '#title' => t('Organized by<span class="form-required">*</span>'),
        '#options' => array(''=>'Select') + organized(),
        '#default_value' =>$organized_by,    
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
      
     $form['registration'] = array(
            '#id' => 'registration',
            '#type' => 'radios',
            '#title' => t('Is this event have student registration?'), 
            '#options' => array(
                1 => t('Yes'), 
                0 => t('No')
                ),
            //'#required' => TRUE,
            '#default_value' => $registration,
            
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
        '#markup' => t('<a  class="experience_calcel" href="'.$base_url.'/'.get_admin_link().'eventsandnews/events">Cancel</a>'),
        '#suffix' => '</div>',
    );
 
 

    return $form;
}


function events_news_update_event_validate($form, &$form_state) {   
    
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


function events_news_update_event_submit($form, &$form_state) {
    global $user;
    extract($form_state['input']);
    $description = $description['value'];
    $params = array(
       // 'event_code' => $event_code,
        'title' => $event_title,
        'from_date' => date('Y-m-d',strtotime($from_date)),
        'from_time' => $from_time,
        'to_date' => date('Y-m-d',strtotime($to_date)),
        'to_time' => $to_time,
        'location' => $location,
        'country' => $country,        
        'description' => $description,
        'note' => $note,
        'organized_by' => $organized_by,
        'registration_required' => $registration,
        'uid' => $user->uid,
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
        $params['photo'] = $dest_photo;
    } 
    
    db_update('events')
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
                $params = array('event_id'=>$eid,'attachment'=>$dest_photo);
                db_insert('event_attachments')
                    ->fields($params)
                    ->execute(); 
            }
        }
    }
    audit_log('Event Updated', 'Event ID:'.sprintf("EV-%04d", $eid));   
    drupal_set_message('Event Updated.');
    drupal_goto(get_admin_link().'eventsandnews/events');
}
