<?php 
function faculty($form, &$form_state){
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
        dateFormat: "dd/mm/yy",
        showOn: "both",
        yearRange: "-100:+0",
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
    $form = array(); 
   
    $form['div'] = array(
        '#type' => 'item', 
        '#prefix' => '<div id="news_list">',
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
        '#submit' => array('faculty_submit'),
        '#prefix' => '<div class="submit_clear_but">',
    );

    $form['div']['cancel'] = array(
        '#type' => 'item', 
        '#markup' => t('<a  class="experience_calcel" href="'.$base_url.'/'.get_admin_link().'eventsandnews/news">Clear</a>'),
        '#suffix' => '</div></div>',
    ); 
    
    $header = array(
            array('data' => t('ID'), 'width' => '5%'), 
            array('data' => t('Name'), 'width' => '10%'),
            array('data' => t('Username'), 'width' => '15%'),
            array('data' => t('Photo'), 'width' => '5%'),
            array('data' => t('Role'), 'width' => '5%'),
            array('data' => t('Type'), 'width' => '5%'),
            array('data' => t('Email'), 'width' => '10%'),
            array('data' => t('DOB'), 'width' => '10%'),
            array('data' => t('Mobile'), 'width' => '10%'),
            array('data' => t('Telephone'), 'width' => '10%'),
            array('data' => t('Cdate'), 'width' => '10%'),            
            array('data' => t('Options'), 'width' => '10%'),
           );
    
    $query = db_select('faculty')
        ->fields('faculty');
    
    
    if (isset($form_state['input']['from_date']) && $form_state['input']['from_date']!="") {
        $query->condition('cdate', date('Y-m-d',strtotime($form_state['input']['from_date'])), '>=');
    }
    
    if (isset($form_state['input']['to_date']) && $form_state['input']['to_date']!="") {
        $query->condition('cdate', date('Y-m-d',strtotime($form_state['input']['to_date'])), '<=');
    }
    
    if (isset($form_state['input']['status']) && $form_state['input']['status']!="") {
        $query->condition('status', $form_state['input']['status'], '=');
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
            $name = $row->name; 
            $username = $row->username; 
            $photo = $row->photo;
            $role = $row->role;
            $type = $row->type;
            $email = $row->email;
            $mobile = $row->mobile;
            $telephone = $row->telephone;
            $dob = date('m/d/Y',strtotime($row->dob));
            $cdate = date('m/d/Y',strtotime($row->cdate));
            $status = $row->status?'Approved':'Pending';
            $approve1 = '<a href="javascript:void(0);" style="cursor:auto;">'.$status."</a>";
            if($photo!='')
                $image = ' <img src="'.file_create_url($photo).'"/>';
            else
                $image = '';
            
            if($approve=='Approve') {
                if($row->status==0){
                    $confirm = "Approve User?";                    
                    $approve1 = '<a href="'.$base_url.'/'.get_admin_link().'approvefaculty/'.encrypt($id,variable_get('IIUM_Encryption_Key', NULL)).'/'.encrypt($row->status,variable_get('IIUM_Encryption_Key', NULL)).'" onclick="return confirm(\''.$confirm.'\');">'.$status.'</a>&nbsp';
                }
                else{
                    $confirm = "Make User Pending?";
                    $approve1 = '<a href="'.$base_url.'/'.get_admin_link().'approvefaculty/'.encrypt($id,variable_get('IIUM_Encryption_Key', NULL)).'/'.encrypt($row->status,variable_get('IIUM_Encryption_Key', NULL)).'" onclick="return confirm(\''.$confirm.'\');">'.$status.'</a>&nbsp';
                }
            }
            $rows[] = array($id,$name,$username,$image,$role,$type,$email,$dob,$mobile,$telephone,$cdate,
                '<div class="options">'.$approve1 
                .'</div>');
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


function faculty_submit($form, &$form_state) {      
    $form_state['rebuild'] = TRUE;
}



