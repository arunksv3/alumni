<?php 
function events_news_events($form, &$form_state){
    global $base_url;
    global $user;
    $location_country = 0;
    drupal_add_js(drupal_get_path('theme', 'bootstrap_business') .'/js/jquery.validate.js');    
    drupal_add_js(drupal_get_path('theme', 'bootstrap_business') .'/js/jqueryformvalidation.js');
    drupal_add_js(drupal_get_path('module', 'events_news') .'/js/custom_validations.js');
    drupal_add_css(drupal_get_path('theme', 'bootstrap_business') .'/css/jquery-ui.css');  
    drupal_add_js(drupal_get_path('theme', 'bootstrap_business') .'/js/jquery-ui.js');
     drupal_add_js('jQuery(document).ready(function($) {         

        $("#from_date").datepicker({
        changeMonth: true,
        changeYear: true,
        showOn: "both",
        dateFormat:"dd/mm/yy",
        yearRange: "-10:+0",
        buttonImage: "'.file_create_url('public://calendar.gif').'",
        buttonImageOnly: true,
        onSelect: function(selected) {
        }
        });
        
        $("#to_date").datepicker({
        changeMonth: true,
        changeYear: true,
        showOn: "both",
        dateFormat:"dd/mm/yy",
        yearRange: "-10:+0",
        buttonImage: "'.file_create_url('public://calendar.gif').'",
        buttonImageOnly: true,
        onSelect: function(selected) {
        }
        });
        
        });', 'inline');
   
    
    $form = array(); 
    
    
    $form['div'] = array(
        '#type' => 'item', 
        '#prefix' => '<div id="events_list">',
    );
  
    
    /*$form['add'] = array(
        '#type' => 'item', 
        '#markup' => t('<a  class="experience_calcel" href="'.$base_url.'/'.get_admin_link().'eventsandnews/addevent">Add</a>'),
        '#suffix' => '</div>',
    );*/
   /* $form['div']['event_code'] = array(
        '#id' => 'event_code',
        '#title' => 'Event Code',        
        '#type' => 'textfield',         
        '#default_value' =>'',  
     );*/
    
    $form['div']['event_title'] = array(
        '#id' => 'event_title',
        '#title' => 'Event Title',        
        '#type' => 'textfield',         
        '#default_value' =>'',  
     );
    
    $form['div']['organized_by'] = array(
        '#id' => 'organized_by',
        '#type' => 'select',
        '#title' => t('Organized by'),
        '#options' => array(''=>'Select') + organized(),
        '#default_value' =>'',  
    );
    $form['div']['date'] = array(
        '#type' => 'item', 
        '#title' => 'Date',
    );
    $form['div']['date']['from_date'] = array(
        '#id' => 'from_date',
        '#title' => 'From',        
        '#type' => 'textfield',   
        '#attributes' => array('readonly'=>'readonly'),
        '#default_value' =>'',  
     );
    
    $form['div']['date']['to_date'] = array(
        '#id' => 'to_date',
        '#title' => 'To',        
        '#type' => 'textfield',   
        '#attributes' => array('readonly'=>'readonly'),
        '#default_value' =>'',  
     );
     
    $form['div']['registration'] = array(
        '#id' => 'registration',
        '#type' => 'select',
        '#title' => t('User Registrations'),
        '#options' => array(''=>'Select','1'=>'Yes','0'=>'No'),
        '#default_value' =>'',  
    );
     
    $form['div']['status'] = array(
        '#id' => 'status',
        '#type' => 'select',
        '#title' => t('Event Status'),
        '#options' => array(''=>'Select','0'=>'Pending','1'=>'Approved'),
        '#default_value' =>'',  
    );
    
    $form['div']['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Search'),
        '#submit' => array('events_submit'),
        '#prefix' => '<div class="submit_clear_but">',
    );

    $form['div']['cancel'] = array(
        '#type' => 'item', 
        '#markup' => t('<a  class="experience_calcel" href="'.$base_url.'/'.get_admin_link().'eventsandnews/events">Clear</a>'),
        '#suffix' => '</div></div>',
    ); 
    
    if($user->uid == 1){
        $header = array(             
            array('data' => t('Event ID'), 'width' => '5%'), 
            //array('data' => t('Event Code'), 'width' => '5%'),
            array('data' => t('Title'), 'width' => '30%'),
            array('data' => t('Date From'), 'width' => '9%'),
            array('data' => t('To'), 'width' => '9%'),
            array('data' => t('Event Status'), 'width' => '11%'),
           // array('data' => t('Photo'), 'width' => '10%'),
            array('data' => t('Organized by'), 'width' => '11%'),
            array('data' => t('Name'), 'width' => '10%'),
            array('data' => t('User Registration'), 'width' => '5%'),
            array('data' => t('Options'), 'width' => '5%'),
        );
    }else{
        $header = array(
            array('data' => t('Event ID'), 'width' => '5%'), 
            //array('data' => t('Event Code'), 'width' => '5%'),
            array('data' => t('Title'), 'width' => '40%'),
            array('data' => t('Date From'), 'width' => '9%'),
            array('data' => t('To'), 'width' => '9%'),
            array('data' => t('Event Status'), 'width' => '11%'),
           // array('data' => t('Photo'), 'width' => '10%'),
            array('data' => t('Organized by'), 'width' => '11%'),
            array('data' => t('User Registration'), 'width' => '5%'),
            array('data' => t('Options'), 'width' => '5%'),
        );
    }
    
    $query = db_select('events')
        ->fields('events');
       
     
//    if (isset($form_state['input']['event_code']) && $form_state['input']['event_code']!="") {
//        $query->condition('event_code',$form_state['input']['event_code'], '=');
//    }
    
    if (isset($form_state['input']['event_title']) && $form_state['input']['event_title']!="") {
        $query->condition('title', '%' . db_like($form_state['input']['event_title']) . '%', 'LIKE');
    }
    
    if (isset($form_state['input']['organized_by']) && $form_state['input']['organized_by']!="") {
        $query->condition('organized_by',$form_state['input']['organized_by'], '=');
    }
    
    if (isset($form_state['input']['from_date']) && $form_state['input']['from_date']!="") {
        $from_input = explode('/', $form_state['input']['from_date']);
        $from_date = $from_input[2] . '-' . $from_input[1] . '-' . $from_input[0]; 
        $query->condition('from_date', $from_date, '>=');
    }
    
    if (isset($form_state['input']['to_date']) && $form_state['input']['to_date']!="") {        
        $to_input = explode('/', $form_state['input']['to_date']);
        $to_date = $to_input[2] . '-' . $to_input[1] . '-' . $to_input[0];  
        $query->condition('to_date', $to_date, '<=');
    }
    
    if (isset($form_state['input']['registration']) && $form_state['input']['registration']!="") {
        $query->condition('registration_required',$form_state['input']['registration'], '=');
    }
    
    if (isset($form_state['input']['status']) && $form_state['input']['status']!="") {
        $query->condition('status',$form_state['input']['status'], '=');
    }
 
    $table_sort = $query->extend('TableSort') // Add table sort extender.
        ->orderByHeader($header); // Add order by headers.

   $pager = $table_sort->extend('PagerDefault')
        ->orderBy('id', 'DESC')
        ->limit(10); // 10 rows per page.

    $result = $pager->execute();
    $rows = array();
    $approve = '';
    if (user_access('approve_events_news', $user)) {
        $approve = 'Approve';
    }
    
    $orgArr = organized();
    foreach($result as $row){
        $id = $row->id;
        //$event_code = $row->event_code;
        $title = $row->title;
        $from_date = $row->from_date;
        $from_time = $row->from_time;
        $to_date = $row->to_date;
        $to_time = $row->to_time;
        $organizedby = $orgArr[$row->organized_by];
        $registration = $row->registration_required ? 'Yes' : 'No';
        $location = $row->location;
        $country = get_country_name($row->country);
        $state = get_state_name($row->state);
        $photo = $row->photo;
        $status = $row->status?'Approved':'Pending';
        $approve1 = '';
        if($photo!='')
            $image = ' <img src="'.file_create_url($photo).'" height="80" width="80"/>';
        else
            $image = '<img src="'.file_create_url('public://uploads/profile_pictures/noimage.jpg').'"/>';
        $description = $row->description;
        $note = $row->note;
         
        if($approve=='Approve') {
            if($row->status==0){
                $confirm = "Approve Event?";
                $approve1 = '<a href="'.$base_url.'/'.get_admin_link().'approveevent/'.encrypt($id,variable_get('IIUM_Encryption_Key', NULL)).'/'.encrypt($row->status,variable_get('IIUM_Encryption_Key', NULL)).'" onclick="return confirm(\''.$confirm.'\')">'.$status.'</a>&nbsp';
            }
            /*else{
                $confirm = "Make Event Pending?";
                //$approve1 = '<a href="'.$base_url.'/'.get_admin_link().'approveevent/'.encrypt($id,variable_get('IIUM_Encryption_Key', NULL)).'/'.encrypt($row->status,variable_get('IIUM_Encryption_Key', NULL)).'" onclick="return confirm(\''.$confirm.'\')">'.$status.'</a>&nbsp';            
            }*/
        }
        if($user->uid==1){ 
            $nameArr = user_load($row->uid);
            $username = $nameArr->name;

            $rows[] = array(sprintf("EV-%04d", $id),$title, date('d/m/Y H:iA',strtotime($from_date.' '.$from_time)),
                    date('d/m/Y H:iA',strtotime($to_date.' '.$to_time)),$status,
                    $organizedby, $username, $registration,
                   '<div class="options">'.$approve1.'<a href="'.$base_url.'/'.get_admin_link().'eventsandnews/editevent/'.encrypt($id,variable_get('IIUM_Encryption_Key', NULL)).'/edit">Edit</a>'
                  .'<a href="javascript:void(0);" onclick="window.open(\''.$base_url.'/'.get_admin_link().'event/viewevent/'.encrypt($id,variable_get('IIUM_Encryption_Key', NULL)).'/view\',\'\',\' scrollbars=yes,menubar=no,width=500, resizable=yes,toolbar=no,location=no,status=no\');">View</a>'
                  .'<a onclick="return confirm(\'Delete Event?\')" href="'.$base_url.'/'.get_admin_link().'deleteevent/'.encrypt($id,variable_get('IIUM_Encryption_Key', NULL)).'/delete">Delete</a></div>');
          }else{
              $rows[] = array(sprintf("EV-%04d", $id),$title,date('d/m/Y H:iA',strtotime($from_date.' '.$from_time)),
                    date('d/m/Y H:iA',strtotime($to_date.' '.$to_time)),$status,
                    $organizedby,$registration,
                   '<div class="options">'.$approve1.'<a href="'.$base_url.'/'.get_admin_link().'eventsandnews/editevent/'.encrypt($id,variable_get('IIUM_Encryption_Key', NULL)).'/edit">Edit</a>'
                  .'<a href="javascript:void(0);" onclick="window.open(\''.$base_url.'/'.get_admin_link().'event/viewevent/'.encrypt($id,variable_get('IIUM_Encryption_Key', NULL)).'/view\',\'\',\' scrollbars=yes,menubar=no,width=500, resizable=yes,toolbar=no,location=no,status=no\');">View</a>'
                  .'<a onclick="return confirm(\'Delete Event?\')" href="'.$base_url.'/'.get_admin_link().'deleteevent/'.encrypt($id,variable_get('IIUM_Encryption_Key', NULL)).'/delete">Delete</a></div>');
          }
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

function events_submit($form, &$form_state) {      
    $form_state['rebuild'] = TRUE;
}
