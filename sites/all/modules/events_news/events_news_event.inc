<?php
function events_news_event($form, &$form_state, $eid){
    global $base_url;
    global $user;
    drupal_add_js(drupal_get_path('theme', 'bootstrap_business') . '/js/jquery.validate.js');
    drupal_add_js(drupal_get_path('theme', 'bootstrap_business') . '/js/jqueryformvalidation.js');
    drupal_add_js(drupal_get_path('module', 'events_news') . '/js/custom_validations.js');
    drupal_add_css(drupal_get_path('theme', 'bootstrap_business') . '/css/jquery-ui.css');
    drupal_add_js(drupal_get_path('theme', 'bootstrap_business') . '/js/jquery-ui.js');
    $decrypting_key=variable_get('IIUM_Encryption_Key', NULL);
    $id=(int)decrypt($eid,$decrypting_key);
    $states_list = alumni_std_states_list();
    $event = db_select('events')
        ->fields('events')
        ->condition('id',$id,'=')
        ->condition('status',1,'=')
        ->execute();
    
    $event_attachments = db_select('event_attachments')
        ->fields('event_attachments')
        ->condition('event_id',$id,'=')
         ->execute();
    $count = $event_attachments->rowCount();
    
    $form = array();
    
    $form['event'] = array(
        '#type' => 'item', 
        '#title' => ''
    );
    $output = '<div class="block block-system clearfix">';
    $registration = false;
    $organizedArr = organized(); 
    foreach($event as $row) {        
        if($row->photo!='')
                $image = ' <img src="'.file_create_url($row->photo).'"/>';
            else
                $image = '<img src="'.file_create_url('public://uploads/profile_pictures/noimage.jpg').'"/>';
            
        $output .='<div class="EN-details-header">
            <span class="date-time">'.date('d M Y',strtotime($row->from_date)).' (Time: '.$row->from_time.') To '.date('d M Y',strtotime($row->to_date)).' (Time: '.$row->to_time.') </span>
            <h1>'.$row->title.'</h1>          
            <div class="EN-header-right">
                
            </div>
        </div>
        <div class="EN-details-content">
        <ul class="en-details-list">
            
           <li>
                <label>Organized By:</label>
                <span>'.$organizedArr[$row->organized_by].'</span> 
            </li>
            <li>
                <label>Venue:</label> 
                <span>'.$row->location.' '.get_country_name($row->country).' '.get_state_name($row->state).'</span>
            </li>
        </ul>
        <div class="EN-content-img">
            '.$image.'
        </div>
        <p>'.$row->description.'</p>'; 
        if($count>0) {
        $output .='<ul class="en-details-list">
        <li>Attachment:';
            foreach($event_attachments as $res) {
                $output .='
                    <a href="'.file_create_url($res->attachment).'" target="_blank">download</a>
                 </li>';
            }
            $output .='</ul>'; 
        }
        $output .='</div>';
        $registration = $row->registration_required;
    }
    
    $output .= '</div>';
    
    $form['events']['list'] = array(
        '#type' => 'item',         
        '#markup' => $output,         
     ); 
    $role = get_user_role($user->uid);
    if($registration && $user->uid > 0 && $role=='student'){
        
        $query = db_select('student_details');
        $query->fields('student_details');
        $query->condition('uid', $user->uid, '=');
        $studentDetails = $query->execute()->fetchAll();        
        $qualification = $studentDetails[0]->highest_qualification ? $studentDetails[0]->highest_qualification : '';
        $qualification_level = $studentDetails[0]->highest_qualification_level ? $studentDetails[0]->highest_qualification_level : '';
        $stdinfo = get_stdinfo($user->uid);
        $nationality = $stdinfo['ctzship'];
        
        $form['image'] = array(
            '#type' => 'item',  
            '#markup' => '<img src="'.file_create_url('public://register-for-this-event.png').'">',
        ); 
        
        
        $form['title'] = array(
            '#type' => 'item',  
            '#title' => '<h2>Participant\'s Particulars - <span>Please fill the form below to register for these event.</span></h2>',         
            '#prefix'=>'<div class="event_details">'
        ); 
         
        $form['name'] = array(
            '#type' => 'textfield',
            '#id' => 'name',
            '#title' => 'Participants Name <span class="form-required">*</span>',
            '#default_value' => $stdinfo['name'],
            '#attributes' => array('readonly'=>'readonly')
        );
        
        $form['icno'] = array(
            '#type' => 'textfield',
            '#id' => 'icno',
            '#maxlength'=>'50',
            '#title' => 'I/C no',
            '#default_value' => $stdinfo['ic_no'],
            '#attributes' => array('readonly'=>'readonly')
        );
        
        $form['email'] = array(
            '#type' => 'textfield',
            '#id' => 'email',
            '#title' => 'Email <span class="form-required">*</span>',
            '#default_value' => $stdinfo['alt_email'],
            '#attributes' => array('readonly'=>'readonly')
        );
        
        $form['eid'] = array(
            '#type' => 'hidden',
            '#default_value' => $eid,
        );
        
       /* $form['designation'] = array(
            '#type' => 'textfield',
            '#id' => 'designation',
            '#title' => 'Designation',
        );*/
        
        $form['highest']['qualification'] = array(
            '#type' => 'select',
            '#options' =>  array(''=>'Select') + qualification_list(),
            '#id' => 'qualification',
            '#title' => 'Highest Qualification',
            '#ajax' => array(
            // 'event' => 'change',
            'callback' => 'ajax_qualification_level_callback',
            'wrapper' => 'dropdown-highest-degree',
          ),
            '#default_value' => $qualification,
        );
        
            $selected = isset($form_state['values']['qualification'])?$form_state['values']['qualification']:'' ;
            $form['highest']['qualification_level'] = array(
            '#id' => 'qualification_level',
            '#type' => 'select',
            //'#required' => TRUE,
            '#title' =>  t('Highest Qualification Level'),
            '#prefix' => '<div id="dropdown-highest-degree">',
            '#suffix' => '</div>',
            '#options' => array(''=>'Select') +highest_qualification_list($selected),
            '#default_value' => $qualification_level,
        ); 
        
         /*
        $form['membership_type'] = array(
            '#type' => 'select',
            '#options' => array(''=>'Select') + membership_type(),
            '#id' => 'membership_type',
            '#title' => 'Membership Type <span class="form-required">*</span>',
        );
       
        $form['company_name'] = array(
            '#type' => 'textfield',
            '#id' => 'company_name',
            '#title' => 'Company Name',
        );
        
        $form['company_address'] = array(
            '#type' => 'textfield',
            '#id' => 'company_address',
            '#title' => 'Company Address',
        );
        
        $form['postal_code'] = array(
            '#type' => 'textfield',
            '#id' => 'postal_code',
            '#title' => 'Postal Code',
            '#maxlength'=>'25'
        );
        */
        $form['mobile'] = array(
            '#type' => 'textfield',
            '#id' => 'mobile',
            '#title' => 'Mobile <span class="form-required">*</span>',
            '#maxlength'=>'15',
            '#default_value' => $stdinfo['contact_mobile'],
            '#attributes' => array('readonly'=>'readonly')
        );
        
        $form['telephone'] = array(
            '#type' => 'textfield',
            '#id' => 'telephone',
            '#title' => 'Telephone',
            '#maxlength'=>'15',
            '#default_value' => $stdinfo['contact_phone'],
            '#attributes' => array('readonly'=>'readonly')
        );
                
        $form['gender'] = array(
            '#id' => 'gender',
            '#type' => 'radios',
            '#title' => t('Gender'), 
            '#options' => array(
                'M' => t('Male'), 
                'F' => t('Female')
                ),
            '#default_value' => $stdinfo['gender'],
            '#attributes' => array('readonly'=>'readonly')
        );
        
        $form['nationality'] = array(
            '#type' => 'textfield',            
            '#id' => 'nationality',
            '#title' => 'Nationality',
            '#default_value' => $states_list[$nationality],
            '#attributes' => array('readonly'=>'readonly')
        );
        /*
        $form['batch_no'] = array(
            '#type' => 'textfield',
            '#id' => 'batch_no',
            '#title' => 'Batch No',
            '#maxlength'=>'50',
            '#default_value' => $stdinfo['batch_no'],
        );*/
        
        $form['submit'] = array(
            '#type' => 'submit',
            '#value' => t('Submit'),   
            '#suffix'=>'</div>'
         ); 
        
    } else {  
        
        $form['script'] = array(
            '#type' => 'item',  
            '#markup' => '<script type="text/javascript">
            jQuery(document).ready(function($) {
               $(".captcha").hide(); 
            });
        </script>',
        ); 
    }
    
     return $form;
    
}

function events_news_event_submit($form, &$form_state){     
    extract($form_state['input']);
    if(!isset($nationality) || $nationality=='')
        $nationality=0;
    
    if(!isset($qualification) || $qualification=='')
        $qualification=0;
    
    if(!isset($qualification_level) || $qualification_level=='')
        $qualification_level=0;
    $decrypting_key=variable_get('IIUM_Encryption_Key', NULL);
    $id=(int)decrypt($eid,$decrypting_key);
    $params = array(
        'event_id' => $id,
        'name' => $name,
        'icno' => $icno,
        'email' => $email,
        'designation' => $designation,
        'qualification' => $qualification,
        'qualification_level' => $qualification_level,
        'membership_type' => $membership_type,
        'company_name' => $company_name,
        'company_address' => $company_address,
        'postal_code' => $postal_code,
        'mobile' => $mobile,
        'telephone' => $telephone,
        'gender' => $gender,
        'nationality' => $nationality,
        'batchno' => $batch_no,
        'created_date' => date('Y-m-d H:i:s')
    );
    
    $record = db_select('event_registrations')
                ->fields('event_registrations')
                ->condition('event_id', $id, '=')
                ->execute()->rowCount();
    
    if($record == 0){    
        db_insert('event_registrations')
            ->fields($params)
            ->execute();
        audit_log('insert', 'User registered for event Event ID:'.sprintf("EV-%04d", $id));   
        drupal_set_message('Your event registration request is in pending approval from admin, you will be notified to your email address once approved. Thanks for your intrest on this event');
    }else{
        drupal_set_message('You already registered for this event','error'); 
    }
}
