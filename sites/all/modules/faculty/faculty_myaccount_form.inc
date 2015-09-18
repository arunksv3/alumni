<?php 
function faculty_myaccount_form($form, &$form_state){
    global $base_url;
    global $user;
    //print_r($user);
    $location_country = 0;
    drupal_add_js(drupal_get_path('theme', 'bootstrap_business') .'/js/jquery.validate.js');    
    drupal_add_js(drupal_get_path('theme', 'bootstrap_business') .'/js/jqueryformvalidation.js');
    drupal_add_js(drupal_get_path('module', 'faculty') .'/js/custom_validations.js');
    drupal_add_css(drupal_get_path('theme', 'bootstrap_business') .'/css/jquery-ui.css');  
    drupal_add_js(drupal_get_path('theme', 'bootstrap_business') .'/js/jquery-ui.js');
    drupal_add_js('jQuery(document).ready(function($) { 
       
        $("#dob").datepicker({
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
        
        });', 'inline');
     
   
    $query = db_select('users_roles', 'u');
    $query->join('role', 'r', 'r.rid = u.rid');
    $query->fields('r');
    $query->condition('u.uid', $user->uid, '=');
    $result = $query->execute()->fetchAll();     
    //$role = $result[0]->name; 
    $email = $user->mail; 
    $user_result = db_select('ldap_user_info')
        ->fields('ldap_user_info')    
        ->condition('uid', $user->uid, '=')
        ->execute();
    $result = $user_result->fetchAssoc();
    $orcldefaultprofilegroup = $result['orcldefaultprofilegroup'];
    $arr = explode(',', $orcldefaultprofilegroup);            
    $cn_arr = explode('=', $arr['0']);
    $cn_value = $cn_arr['1'];
    $user_type_arr = explode('_', $cn_value);   
    $ldap_value = $user_type_arr['1'];
    $ldap_type = $user_type_arr['2'];
                        
    $name = $result['givenname'];
    $query = db_select('faculty')
        ->fields('faculty')    
        ->condition('uid', $user->uid, '=')
        ->execute();  
    
    $count = $query->rowCount(); 
    if($count>0){
        drupal_goto(get_admin_link()."faculty/myaccount");
    }
    
    $form = array();
    
   /* $form['title']= array(
        '#type' => 'item',
        '#markup' => '<h2>Update Details</h2>'
           
    );*/
    
    $form['application_id'] = array(
     '#type' => 'textfield', 
     '#id' => 'application_id',
     '#title' => 'Application ID <span class="form-required">*</span>',
     '#attributes' => array('readonly' => 'readonly'),
     '#default_value' => $user->uid   
    );
    
    $form['name'] = array(
     '#type' => 'textfield', 
     '#id' => 'name',
     '#description' => 'this value will be coming from existing database',
     '#title' => 'Name <span class="form-required">*</span>',
     '#attributes' => array('readonly' => 'readonly'),
     '#default_value' => $name
    );
    
    $form['username'] = array(
     '#type' => 'textfield', 
     '#id' => 'username',
     '#description' => 'this value will be coming from existing database',        
     '#title' => 'Username <span class="form-required">*</span>',
     '#attributes' => array('readonly' => 'readonly'),
     '#default_value' => $user->mail
    );
    
    $form['ldap_value'] = array(
     '#type' => 'textfield', 
     '#id' => 'ldap_value',
     '#description' => 'this value will be coming from existing database',        
     '#title' => 'Ldap Value <span class="form-required">*</span>',
     '#attributes' => array('readonly' => 'readonly'),
     '#default_value' => $ldap_value
    );
    
    $form['ldap_type'] = array(
     '#type' => 'textfield', 
     '#id' => 'type',
     '#description' => 'this value will be coming from existing database',        
     '#title' => 'Ldap Type <span class="form-required">*</span>',
     '#attributes' => array('readonly' => 'readonly'),
    '#default_value' => $ldap_type
    );
    
    $form['email'] = array(
     '#type' => 'textfield', 
     '#id' => 'email',
        '#attributes' => array('readonly' => 'readonly'),
     '#description' => 'this value will be coming from existing database',        
     '#title' => 'Email <span class="form-required">*</span>',   
        '#default_value' => $email
    );
    
    $form['dob'] = array(
     '#type' => 'textfield', 
     '#id' => 'dob',
     '#title' => 'Date of Birth <span class="form-required">*</span>',
        '#attributes' => array('readonly' => 'readonly'),
    );
    
    $form['photo'] = array(
     '#type' => 'file', 
     '#id' => 'photo',
     '#title' => 'Photo',        
    );
    
    $form['mobile'] = array(
     '#type' => 'textfield', 
     '#id' => 'mobile',
     '#title' => 'Mobile <span class="form-required">*</span>',   
    );
    
    $form['telephone'] = array(
     '#type' => 'textfield', 
     '#id' => 'telephone',
     '#title' => 'Telephone <span class="form-required">*</span>',   
    );
    
    $form['nationality'] = array(
     '#type' => 'select', 
     '#options' => array(''=>'Select') + countries_list(),
     '#id' => 'nationality',
     '#title' => 'Nationality <span class="form-required">*</span>',   
    );
    
    $form['country'] = array(
        '#type' => 'select',
        '#title' => t('Country<span class="form-required">*</span>'),
        '#options' => array(''=>'Select') + countries_list(),
        '#ajax' => array(
            'event' => 'change',
            'callback' => 'employer_registration_state_list_ajax',
            'wrapper' => 'state-list-wrapper',
        ),
        '#default_value' => (isset($form_state['input']['country']) ? $form_state['input']['country'] : ''),
    );
    
    // Wrapper for state dropdown list
    $form['address']['wrapper'] = array(
        '#prefix' => '<div id="state-list-wrapper">',
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
            $form['address']['wrapper']['state'] = array(
                '#type' => 'select',
                '#title' => t('State<span class="form-required">*</span>'),
                 '#options' => $state_list_arr,
                '#default_value' => (isset($form_state['input']['state']) ? $form_state['input']['state'] : ''),            
            );
        }
    }
    
    
    
    $form['department'] = array(
     '#type' => 'textfield', 
     '#id' => 'department',
     '#title' => 'Department <span class="form-required">*</span>',   
    );
    
    $form['physical_department'] = array(
     '#type' => 'textfield', 
     '#id' => 'physical_department',
     '#title' => 'Physical Department <span class="form-required">*</span>',   
    );
    
    $form['additional_info'] = array(
     '#type' => 'textarea', 
     '#id' => 'additional_info',
     '#title' => 'Additional Info <span class="form-required">*</span>',   
    );
    
    
   $form['submit'] = array(
            '#type' => 'submit',
            '#value' => t('Submit'),   
            '#prefix' => '<div class="submit_clear_but">',
        );
      
    $form['cancel'] = array(
        '#type' => 'item', 
        '#markup' => t('<a  class="experience_calcel" href="'.$base_url. '/'.get_admin_link().'faculty/account">Cancel</a>'),        
        '#suffix' => '</div>',
    ); 
    return $form;
}


function faculty_myaccount_form_submit($form, &$form_state) {
    global $user;
    extract($form_state['input']);   
    if(!isset($state) || $state=='')
        $state = 0;
    
    $date = explode('/', $dob);    
    $dob = $date[2].'-'.$date[1].'-'.$date[0];
    
    $arr = array(
           'email' => $email,
           'name' => $name,
           'username' => $username,
           'ldap_value' => $ldap_value,
           'ldap_type' => $ldap_type,
           'dob' => $dob,
           'mobile' => $mobile,
           'telephone' => $telephone,            
           'nationality' => $nationality,
           'country' => $country,
           'state' => $state,            
           'additional_info' => $additional_info,
           'department' => $department,
           'physical_department' => $physical_department,
           'uid' => $user->uid,
           'cdate' => date('Y-m-d H:i:s')
    );
    
    if (isset($_FILES["files"]["name"]['photo']) && $_FILES["files"]["name"]['photo'] != '') {
        $photo_name = $_FILES["files"]["name"]['photo'];
        // Move files to destination folder
        $photo_temp = $_FILES["files"]["tmp_name"]['photo'];
        $photo = time() . '_' . $photo_name;
        $dest_photo = 'public://uploads/profile_pictures/' . $photo;
        move_uploaded_file($photo_temp, $dest_photo);
        $arr['photo'] = $dest_photo;
    }

    db_insert('faculty')
        ->fields($arr)
        ->execute();
    
    db_update('users')
    ->fields(array('status'=>0))
    ->condition('uid', $user->uid, '=')
    ->execute();
    
    audit_log('update', 'Faculty Details Updated.');
    drupal_set_message('Your account details will be verified by system admin, and you would be able to login to your account after approval. you will be notified to the email address that is associated with this account.');
    drupal_goto('user');
}