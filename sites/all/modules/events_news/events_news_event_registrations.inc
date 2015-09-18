<?php 
function events_news_event_registrations($form, &$form_state){
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
        yearRange: "-10:+0",
        dateFormat:"dd/mm/yy",
        buttonImage: "'.file_create_url('public://calendar.gif').'",
        buttonImageOnly: true,
        onSelect: function(selected) {
        }
        });
        
        $("#to_date").datepicker({
        changeMonth: true,
        changeYear: true,
        showOn: "both",
        yearRange: "-10:+0",
        dateFormat:"dd/mm/yy",
        buttonImage: "'.file_create_url('public://calendar.gif').'",
        buttonImageOnly: true,
        onSelect: function(selected) {
        }
        });
        
        });', 'inline');
   
    
    $form = array(); 
    
     $form['div'] = array(
        '#type' => 'item', 
        '#prefix' => '<div id="event_registrations">',
    );
   
   /*
    $form['div']['event_code'] = array(
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
    
    $form['div']['date'] = array(
        '#type' => 'item', 
        '#title' => 'User Registration Date', 
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
     
    $form['div']['email'] = array(
        '#id' => 'email',
        '#type' => 'textfield',
        '#title' => t('Email Address'),
        '#default_value' =>'',  
    );
    
    $form['div']['name'] = array(
        '#id' => 'name',
        '#type' => 'textfield',
        '#title' => t('Participant Name'),
        '#default_value' =>'',  
    );
   
     
    $form['div']['status'] = array(
        '#id' => 'status',
        '#type' => 'select',
        '#title' => t('Registration Status'),
        '#options' => array(''=>'Select','0'=>'Pending','1'=>'Approved','2'=>'Rejected'),
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
        '#markup' => t('<a  class="experience_calcel" href="'.$base_url.'/'.get_admin_link().'eventsandnews/event_registrations">Clear</a>'),
        '#suffix' => '</div></div>',
    ); 
    
    $header = array(
        array('data' => t('Registration ID'), 'width' => '12%'), 
        array('data' => t('Participant Name'), 'width' => '18%'), 
       // array('data' => t('Event Code'), 'width' => '5%'),
        array('data' => t('Event Title'), 'width' => '20%'),
        array('data' => t('Registration Date'), 'width' => '15%'),        
        array('data' => t('Status'), 'width' => '5%'),       
        array('data' => t('Email Address'), 'width' => '20%'),
        array('data' => t('Options'), 'width' => '5%'),
    );
    
    $query = db_select('event_registrations', 'r');
    $query->join('events','e','e.id=r.event_id');
    $query->fields('r',array());
    $query->fields('e',array('event_code','title'));
       
     /*
    if (isset($form_state['input']['event_code']) && $form_state['input']['event_code']!="") {
        $query->condition('event_code',$form_state['input']['event_code'], '=');
    }*/
    
    if (isset($form_state['input']['event_title']) && $form_state['input']['event_title']!="") {
        $query->condition('title', '%' . db_like($form_state['input']['event_title']) . '%', 'LIKE');
    } 
    
    if (isset($form_state['input']['from_date']) && $form_state['input']['from_date']!="") {
        $from_input = explode('/', $form_state['input']['from_date']);
        $from_date = $from_input[2] . '-' . $from_input[1] . '-' . $from_input[0]; 
        $query->condition('r.created_date', $from_date, '>=');
    }
    if (isset($form_state['input']['to_date']) && $form_state['input']['to_date']!="") {
        $to_input = explode('/', $form_state['input']['to_date']);
        $to_date = $to_input[2] . '-' . $to_input[1] . '-' . $to_input[0];    
        $query->condition('r.created_date', $to_date , '<=');
    }
    if (isset($form_state['input']['email']) && $form_state['input']['email']!="") {
        $query->condition('email',$form_state['input']['email'], '=');
    }
    if (isset($form_state['input']['name']) && $form_state['input']['name']!="") {
        $query->condition('name','%'.$form_state['input']['name'].'%', 'LIKE');
    }
    if (isset($form_state['input']['status']) && $form_state['input']['status']!="") {
        $query->condition('r.status',$form_state['input']['status'], '=');
    }
 
    $table_sort = $query->extend('TableSort') // Add table sort extender.
        ->orderByHeader($header); // Add order by headers.

   $pager = $table_sort->extend('PagerDefault')
        ->orderBy('id', 'DESC')
        ->limit(10); // 10 rows per page.

    $result = $pager->execute();
    $rows = array();
     
    
    $orgArr = organized();
    foreach($result as $row){
        $id = $row->id;
        $name = $row->name;
        $email = $row->email;
        //$event_code = $row->event_code;
        $title = $row->title;        
        $date = $row->created_date;
        $approve1 = "";
        if($row->status==0){
            $approve1 = '<a href="'.$base_url.'/'.get_admin_link().'approvereguser/'.encrypt($id,variable_get('IIUM_Encryption_Key', NULL)).'/1" onclick="return confirm(\'Approve User?\')">Approve</a>';
            $approve1 .= '<a href="'.$base_url.'/'.get_admin_link().'approvereguser/'.encrypt($id,variable_get('IIUM_Encryption_Key', NULL)).'/2" onclick="return confirm(\'Reject User?\')">Reject</a>';
            $status = 'Pending';
        } else if($row->status==1){
            //$approve1 = '<a href="javascript:void(0);" style="cursor:auto;">Approve</a><a href="javascript:void(0);" style="cursor:auto;">Reject</a>';
            $status = 'Approved';
        } else{
            //$approve1 = '<a href="javascript:void(0);" style="cursor:auto;">Approve</a><a href="javascript:void(0);" style="cursor:auto;">Reject</a>';
            $status = 'Rejected';
        }
        
         $rows[] = array(sprintf("ER-%04d", $id),$name,$title,date('d/m/Y H:iA',strtotime($date)),
             $status,
            $email, '<div class="options">'.$approve1.
           '<a href="javascript:void(0);" onclick="window.open(\''.$base_url.'/'.get_admin_link().'event/viewreguser/'.encrypt($id,variable_get('IIUM_Encryption_Key', NULL)).'/view\',\'\',\' scrollbars=yes,menubar=no,width=600, resizable=yes,toolbar=no,location=no,status=no\');">View</a>'
         . '<a onclick="return confirm(\'Delete Registration?\')" href="'.$base_url.'/'.get_admin_link().'deletereguser/'.encrypt($id,variable_get('IIUM_Encryption_Key', NULL)).'/delete">Delete</a></div>');
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
