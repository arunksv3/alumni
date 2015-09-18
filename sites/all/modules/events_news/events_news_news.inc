<?php 
function events_news_news($form, &$form_state){
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
    
    
    /*
    $form['add'] = array(
        '#type' => 'item', 
        '#markup' => t('<a  class="experience_calcel" href="'.$base_url.'/'.get_admin_link().'eventsandnews/addnews">Add</a>'),
        '#suffix' => '</div>',
    );*/
    $form['div'] = array(
        '#type' => 'item', 
        '#prefix' => '<div id="news_list">',
    ); 
  
    
    $form['div']['news_title'] = array(
        '#id' => 'news_title',
        '#title' => 'Title',        
        '#type' => 'textfield',         
        '#default_value' =>'',  
     );
    
    $form['div']['reported_by'] = array(
         '#id' => 'reported_by',
        '#type' => 'select',
        '#title' => t('Reported by'),
        '#options' => array(''=>'Select') + reported(),
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
     
     $form['div']['category'] = array(
         '#id' => 'category',
        '#type' => 'select',
        '#title' => t('Category'),
        '#options' => array(''=>'Select') + news_category(),
         '#default_value' =>'',  
    );
     
     $form['div']['status'] = array(
         '#id' => 'status',
        '#type' => 'select',
        '#title' => t('News Status'),
        '#options' => array(''=>'Select','0'=>'Pending','1'=>'Approved'),
         '#default_value' =>'',  
    );
    
     $form['div']['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Search'),
        '#submit' => array('news_submit'),
        '#prefix' => '<div class="submit_clear_but">',
    );

    $form['div']['cancel'] = array(
        '#type' => 'item', 
        '#markup' => t('<a  class="experience_calcel" href="'.$base_url.'/'.get_admin_link().'eventsandnews/news">Clear</a>'),
        '#suffix' => '</div></div>',
    ); 
    
    
      if($user->uid == 1){
          $header = array(                
                array('data' => t('News ID'), 'width' => '5%'), 
                array('data' => t('Title'), 'width' => '45%'),
                array('data' => t('Date'), 'width' => '5%'),
                //array('data' => t('Photo'), 'width' => '12%'),
                array('data' => t('Reported by'), 'width' => '15%'),
                array('data' => t('Name'), 'width' => '10%'), 
                array('data' => t('Status'), 'width' => '5%'),
                array('data' => t('Category'), 'width' => '5%'),
                array('data' => t('Options'), 'width' => '20%'),
            );
      }else{
          $header = array(
                array('data' => t('News ID'), 'width' => '5%'), 
                array('data' => t('Title'), 'width' => '45%'),
                array('data' => t('Date'), 'width' => '5%'),
                //array('data' => t('Photo'), 'width' => '12%'),
                array('data' => t('Reported by'), 'width' => '15%'),
                array('data' => t('Status'), 'width' => '5%'),
                array('data' => t('Category'), 'width' => '5%'),
                array('data' => t('Options'), 'width' => '20%'),
          );
      }
                
    
    $query = db_select('news')
        ->fields('news');
    
    if (isset($form_state['input']['news_title']) && $form_state['input']['news_title']!="") {
        $query->condition('title', '%' . db_like($form_state['input']['news_title']) . '%', 'LIKE');
    }
    
    if (isset($form_state['input']['reported_by']) && $form_state['input']['reported_by']!="") {
        $query->condition('reported_by', $form_state['input']['reported_by'], '=');
    }
    
    if (isset($form_state['input']['from_date']) && $form_state['input']['from_date']!="") {
        $from_input = explode('/', $form_state['input']['from_date']);
        $from_date = $from_input[2] . '-' . $from_input[1] . '-' . $from_input[0]; 
        $query->condition('date', $from_date, '>=');
    }
    
    if (isset($form_state['input']['to_date']) && $form_state['input']['to_date']!="") {
        $to_input = explode('/', $form_state['input']['to_date']);
        $to_date = $to_input[2] . '-' . $to_input[1] . '-' . $to_input[0]; 
        $query->condition('date', $to_date, '<=');
    }
    
    if (isset($form_state['input']['status']) && $form_state['input']['status']!="") {
        $query->condition('status', $form_state['input']['status'], '=');
    }
    
    if (isset($form_state['input']['category']) && $form_state['input']['category']!="") {
        $query->condition('category', $form_state['input']['category'], '=');
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
        $categoryArr = news_category();
        $reportedArr = reported();
        foreach($result as $row){
            $id = $row->id; 
            $title = $row->title; 
            $photo = $row->photo;
            $reportedby = $reportedArr[$row->reported_by];
            $category = $categoryArr[$row->category];
            $date = dateformat($row->date);
            $status = $row->status?'Approved':'Pending';
            $approve1 = '';
            if($photo!='')
                $image = ' <img src="'.file_create_url($photo).'"/>';
            else
                $image = '<img src="'.file_create_url('public://uploads/profile_pictures/noimage.jpg').'"/>';
            $description = $row->description;
            if($approve=='Approve') {
                if($row->status==0){
                    $confirm = "Approve News?";                    
                    $approve1 = '<a href="'.$base_url.'/'.get_admin_link().'approvenews/'.encrypt($id,variable_get('IIUM_Encryption_Key', NULL)).'/'.encrypt($row->status,variable_get('IIUM_Encryption_Key', NULL)).'" onclick="return confirm(\''.$confirm.'\')">'.$status.'</a>&nbsp';
                }
                /*else{
                    $confirm = "Make News Pending?";
                    $approve1 = '<a href="'.$base_url.'/'.get_admin_link().'approvenews/'.encrypt($id,variable_get('IIUM_Encryption_Key', NULL)).'/'.encrypt($row->status,variable_get('IIUM_Encryption_Key', NULL)).'" onclick="return confirm(\''.$confirm.'\')">'.$status.'</a>&nbsp';
                }*/
            }
            if($user->uid==1){ 
                $nameArr = user_load($row->uid);
                $username = $nameArr->name;
                $rows[] = array(sprintf("NW-%04d", $id),$title,$date,$reportedby, $username, $status,$category,
                    '<div class="options">'.$approve1.'<a href="'.$base_url.'/'.get_admin_link().'eventsandnews/editnews/'.encrypt($id,variable_get('IIUM_Encryption_Key', NULL)).'/edit">Edit</a> '
                    .'<a href="javascript:void(0);" onclick="window.open(\''.$base_url.'/'.get_admin_link().'event/viewnews/'.encrypt($id,variable_get('IIUM_Encryption_Key', NULL)).'/view\',\'\',\' scrollbars=yes,menubar=no,width=500, resizable=yes,toolbar=no,location=no,status=no\');">View</a>'
                    .'<a onclick="return confirm(\'Delete News?\')" href="'.$base_url.'/'.get_admin_link().'deletenews/'.encrypt($id,variable_get('IIUM_Encryption_Key', NULL)).'/delete">Delete</a></div>');
            }else{
                 $rows[] = array(sprintf("NW-%04d", $id),$title,$date,$reportedby,$status,$category,
                '<div class="options">'.$approve1.'<a href="'.$base_url.'/'.get_admin_link().'eventsandnews/editnews/'.encrypt($id,variable_get('IIUM_Encryption_Key', NULL)).'/edit">Edit</a> '
                .'<a href="javascript:void(0);" onclick="window.open(\''.$base_url.'/'.get_admin_link().'event/viewnews/'.encrypt($id,variable_get('IIUM_Encryption_Key', NULL)).'/view\',\'\',\' scrollbars=yes,menubar=no,width=500, resizable=yes,toolbar=no,location=no,status=no\');">View</a>'
                .'<a onclick="return confirm(\'Delete News?\')" href="'.$base_url.'/'.get_admin_link().'deletenews/'.encrypt($id,variable_get('IIUM_Encryption_Key', NULL)).'/delete">Delete</a></div>');
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


function news_submit($form, &$form_state) {      
    $form_state['rebuild'] = TRUE;
}



