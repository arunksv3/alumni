<?php
/**
 * Function for handling ajax request
 */
function employer_viewstate_list_ajax($form, $form_state)
{
    //echo $form['filter']['wrapper'];  
   return $form['filter']['wrapper'];  
}
/**
 * Shortlist Function for multiple chechboxes
 */
function employer_shortlist_submit($form, &$form_state)
{  
    global $user; 
    if(isset($_POST['shortlist_checkboxes']) && count($_POST['shortlist_checkboxes'])>0)
    {
      $shortlist_checkboxes=$_POST['shortlist_checkboxes'];
      /*Decrypting the data using configration key*/
      $decrypting_key=variable_get('IIUM_Encryption_Key', NULL);
      foreach($shortlist_checkboxes as $shortlist_checkbox):
        $shortlist_checkboxes_arr[]=(int)decrypt($shortlist_checkbox,$decrypting_key);
        //call studentid jobid function
        $job_studentid_jobid=job_studentid_jobid((int)decrypt($shortlist_checkbox,$decrypting_key));
        $user_status_history=user_status_history($user->uid,$job_studentid_jobid->uid,$job_studentid_jobid->sjid, 1, 'employer');
        audit_log('shortlisted', 'user id: '.$job_studentid_jobid->uid."<br> job id: ".$job_studentid_jobid->sjid); 
      endforeach;
      
      $fields = array('status' => 1);
      $employer_jobs_delete=db_update('student_job_details')
                   ->fields($fields)
                   ->condition('id',$shortlist_checkboxes_arr,'IN')
                   ->execute(); 
      drupal_set_message('Selected Applicant /s are shortlisted, you can request them for interview from "Request Shortlisted applicants for interview" tab.', 'status');
      drupal_goto('employer/viewapplicants');
    }
}
/**
 * Reject Function for multiple chechboxes
 */
function employer_reject_submit($form, &$form_state)
{   
    global $user; 
    if(isset($_POST['shortlist_checkboxes']) && count($_POST['shortlist_checkboxes'])>0)
    {
      $shortlist_checkboxes=$_POST['shortlist_checkboxes'];
      /*Decrypting the data using configration key*/
      $decrypting_key=variable_get('IIUM_Encryption_Key', NULL);
      foreach($shortlist_checkboxes as $shortlist_checkbox):
       $shortlist_checkboxes_arr[]=(int)decrypt($shortlist_checkbox,$decrypting_key);
       //call studentid jobid function
       $job_studentid_jobid=job_studentid_jobid((int)decrypt($shortlist_checkbox,$decrypting_key));
       $user_status_history=user_status_history($user->uid,$job_studentid_jobid->uid,$job_studentid_jobid->sjid, 2, 'employer');
       audit_log('Rejected', 'User ID: '.$job_studentid_jobid->uid."<br>Job ID: ".$job_studentid_jobid->sjid); 
      endforeach;
      
      $fields = array('status' => 2);
      $employer_jobs_delete=db_update('student_job_details')
                   ->fields($fields)
                   ->condition('id',$shortlist_checkboxes_arr,'IN')
                   ->execute(); 
      drupal_set_message('Selected Applicant /s are Rejected.', 'status');
      drupal_goto('employer/viewapplicants');
    }
}
function employer_viewapplicants_submit($form, &$form_state) {
      $form_state['filters']['job_id'] = $form_state['values']['job_id']; 
      $form_state['filters']['job_title'] = $form_state['values']['job_title'];
      $form_state['filters']['from'] = $form_state['values']['from']; 
      $form_state['filters']['to'] = $form_state['values']['to']; 
      $form_state['filters']['applicant_name'] = $form_state['values']['applicant_name']; 
      $form_state['filters']['candidatelocation'] = $form_state['values']['candidatelocation']; 
      $form_state['filters']['candidatestate'] = isset($form_state['values']['candidatestate'])?$form_state['values']['candidatestate']:''; 
      $form_state['filters']['min'] = $form_state['values']['min']; 
      $form_state['filters']['max'] = $form_state['values']['max']; 
      $form_state['filters']['highest_degree'] = $form_state['values']['highest_degree']; 
      $form_state['filters']['status'] = $form_state['values']['status']; 
      $form_state['filters']['year_of_experience_from'] = $form_state['values']['year_of_experience_from']; 
      $form_state['filters']['year_of_experience_to'] = $form_state['values']['year_of_experience_to']; 
      
      $form_state['rebuild'] = TRUE;
    }
/*View applicants function*/
function employer_viewapplicants($form, &$form_state)
{  
   global $user;    
   if(isset($_GET) && isset($_GET['shortlistid']) && $_GET['shortlistid']!=""){
          $fields = array(
            'status' => 1,
           );
          /*Decrypting the data using configration key*/
            $decrypting_key=variable_get('IIUM_Encryption_Key', NULL);
            $shortlistid=(int)decrypt($_GET['shortlistid'],$decrypting_key);
            
          $employer_jobs_shortlist=db_update('student_job_details')
                         ->fields($fields)
                         ->condition('id',$shortlistid)
                         ->execute();
       //call studentid jobid function
       $job_studentid_jobid=job_studentid_jobid($shortlistid);
       $user_status_history=user_status_history($user->uid,$job_studentid_jobid->uid,$job_studentid_jobid->sjid, 1, 'employer');
       audit_log('shortlisted', 'user id: '.$job_studentid_jobid->uid."<br> job id: ".$job_studentid_jobid->sjid); 
       if($user_status_history==TRUE)
       {
        drupal_set_message('Selected Applicant /s are shortlisted, you can request them for interview from "Request Shortlisted applicants for interview" tab.', 'status');
        drupal_goto('employer/viewapplicants');
       } 
     }
   if(isset($_GET) && isset($_GET['rejected']) && $_GET['rejected']!=""){
          $fields = array(
            'status' => 2,
           );
          /*Decrypting the data using configration key*/
            $decrypting_key=variable_get('IIUM_Encryption_Key', NULL);
            $rejectedid=(int)decrypt($_GET['rejected'],$decrypting_key);
            
          $employer_jobs_reject=db_update('student_job_details')
                         ->fields($fields)
                         ->condition('id',$rejectedid)
                         ->execute(); 
          
       //call studentid jobid function
       $job_studentid_jobid=job_studentid_jobid($rejectedid);
       $user_status_history=user_status_history($user->uid,$job_studentid_jobid->uid,$job_studentid_jobid->sjid,2,'employer');
       audit_log('rejected', 'user id: '.$job_studentid_jobid->uid."<br>job id: ".$job_studentid_jobid->sjid); 
       if($user_status_history==TRUE)
       {  
        drupal_set_message('Selected Applicant is Rejected. Applicant record will be removed from shortlist, You will find this record back in "View applicants" tab.', 'status');
        drupal_goto('employer/viewapplicants');
       } 
     } 
     drupal_add_css(drupal_get_path('theme', 'bootstrap_business') .'/css/jquery-ui.css');  
     drupal_add_js(drupal_get_path('theme', 'bootstrap_business') .'/js/jquery-ui.js');
     drupal_add_js('jQuery(document).ready(function($) { 
        $("#from").datepicker({
        changeMonth: true,
        changeYear: true,
        showOn: "both",
        dateFormat:"dd/mm/yy",
        buttonImage: "'.file_create_url('public://calendar.gif').'",
        buttonImageOnly: true,
        onSelect: function(selected) {
          $("#to").datepicker("option","minDate", selected)
        }
        });
        $("#to").datepicker({
        changeMonth: true,
        changeYear: true,
        showOn: "both",
        dateFormat:"dd/mm/yy",
        buttonImage: "'.file_create_url('public://calendar.gif').'",
        buttonImageOnly: true,
        onSelect: function(selected) {
         $("#from").datepicker("option","maxDate", selected)
        }
        }); 
        $(".popup").click(function () {                    
          window.open($(this).attr("href"), "PopupWindow", "width=600,height=600,scrollbars=yes,resizable=no");
          return false;
        });
        });', 'inline');
      drupal_add_js("function shortlist(){
        var cnf = confirm('Are you sure you want to shortlist this applicant? Please click Ok to proceed.');
        if(cnf)
            return true;
        else
            return false;    
         }
       function rejected(){
        var cnf = confirm('Are you sure you want to reject this applicant and remove him from shortlist panel? Please click Ok to proceed.');
        if(cnf)
            return true;
        else
            return false;    
         }   
        ",'inline');
    $form = array();
    $form['filter'] = array(
      '#type' => 'fieldset',
    );
    $form['filter']['job_id'] = array(
      '#type' => 'textfield',
      '#title' => t('Job ID'),
    );
    $form['filter']['job_title'] = array(
      '#type' => 'textfield',
      '#title' => t('Job Title'),
    );
    $form['filter']['daterange'] = array(
      '#type' => 'fieldset',
      '#title' => t('Applied On'),
    );
    $form['filter']['daterange']['from'] = array(
    '#type' => 'textfield', 
    '#id' => 'from', 
    '#title' => 'From',
    '#attributes' => array('readonly' => 'readonly'),
    );
    $form['filter']['daterange']['to'] = array(
    '#type' => 'textfield', 
    '#id' => 'to', 
    '#title' => 'To',
    '#attributes' => array('readonly' => 'readonly'),
    );
    $form['filter']['applicant_name'] = array(
      '#type' => 'textfield',
      '#title' => t('Applicant Name'),
    );
    $form['filter']['year_of_experience'] = array(
      '#type' => 'fieldset',
      '#title' => t('Years of Experience'),
    );
    $form['filter']['year_of_experience']['year_of_experience_from'] = array(
    '#type' => 'select', 
    '#id' => 'year_of_experience_from', 
    '#title' => 'From',
    '#options' => yearofexperience(),
    );
    $form['filter']['year_of_experience']['year_of_experience_to'] = array(
    '#type' => 'select', 
    '#id' => 'year_of_experience_to', 
    '#title' => 'To',
    '#options' => yearofexperience(),
    );
    $form['filter']['candidatelocation'] = array(
    '#type' => 'select', 
    '#title' => 'Candidate Location',
    '#options' => array(''=>'Select') + countries_list(),
    '#ajax' => array(
       'event'=>'change',
       'callback' => 'employer_viewstate_list_ajax',
       'wrapper' => 'child-list-wrapper',
      ),    
    );
    // Wrapper for child dropdown list
    $form['filter']['wrapper'] = array(
     '#prefix' => '<div id="child-list-wrapper">',
     '#suffix' => '</div>',
    );
   if (isset($form_state['values']['candidatelocation']) && $form_state['values']['candidatelocation']!="") {
        $state_list_viewapplicants = db_select('states_list')
                ->fields('states_list')
                ->condition('cid',$form_state['values']['candidatelocation'], '=')
                ->execute()
                ->fetchAll();
        $display_viewapplicants['']= 'All';   
        foreach($state_list_viewapplicants as $state_viewapplicant_name): 
          $display_viewapplicants[$state_viewapplicant_name->id]= $state_viewapplicant_name->name;   
        endforeach;
      if(count($state_list_viewapplicants) > 0){   
            // Province dropdown list
        $form['filter']['wrapper']['candidatestate'] = array(
         '#type' => 'select',
         '#options' => $display_viewapplicants,
        );  
      }
    } 
   /*$form['filter']['highest_degree'] = array(
    '#type' => 'select', 
    '#title' => 'Highest degree of Education',
     '#options' => levelofqualifcation(), 
    );
   $form['filter']['age'] = array(
      '#type' => 'fieldset',
      '#title' => t('Candidate Age'),
    );
    $form['filter']['age']['min'] = array(
    '#type' => 'textfield', 
    '#id' => 'min', 
    '#title' => 'Min',
    );
    $form['filter']['age']['max'] = array(
    '#type' => 'textfield', 
    '#id' => 'max', 
    '#title' => 'Max',
    );*/
    $form['filter']['status'] = array(
    '#type' => 'select', 
    '#title' => 'Application Status',
    '#options' => employer_student_status(),
    );
    $form['filter']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Search'),
      '#submit' => array('employer_viewapplicants_submit'),
    );
    $form['actions'] = array(
      '#type' => 'fieldset',
      '#prefix' => '<div class="action_tabs">',
      '#suffix' => '</div>',
    );
    $form['actions']['shortlist'] = array(
      '#type' => 'submit',
      '#value' => t('shortlist'),
      '#submit' => array('employer_shortlist_submit'),
    ); 
    $form['actions']['reject'] = array(
      '#type' => 'submit',
      '#value' => t('Reject'),
      '#submit' => array('employer_reject_submit'),
    ); 
    
    
   $header = array(
             array('data' => t(''), 'width' => '1%'),
             array('data' => t('Applicant ID'), 'width' => '6%'), 
             array('data' => t('Applicant Name'), 'width' => '17%'),
             array('data' => t('Current Exp'), 'width' => '9%'),
             array('data' => t('Location'), 'width' => '8%'),
             array('data' => t('Job ID'), 'width' => '7%'),
             array('data' => t('Job Title'), 'width' => '22%'),
             array('data' => t('Applied On'), 'width' => '5%'),
             array('data' => t('Status'), 'width' => '15%'),
             array('data' => t('Options'), 'width' => '10%'),      
           );
   $query = db_select('student_job_details', 's');
   $query->innerjoin('employer_jobs', 'e', 'e.id = s.sjid');
   $query->leftJoin('stdinfo', 'i', 'i.uid = s.uid');
   $query->fields('s', array('id','uid','sjid','modified_date','country','state','status','experience_in_years','experience_in_months'));
   $query->fields('e', array('jobid','job_title'));
   $query->fields('i', array('name','alt_email'));

   if (isset($form_state['filters']['job_id']) && $form_state['filters']['job_id']!="") {
       $query->condition('e.jobid', '%' . db_like($form_state['filters']['job_id']) . '%', 'LIKE');
   }
   if (isset($form_state['filters']['job_title']) && $form_state['filters']['job_title']!="") {
       $query->condition('e.job_title', '%' . db_like($form_state['filters']['job_title']) . '%', 'LIKE');
   }
   if (isset($form_state['filters']['from'])&& $form_state['filters']['from']!="") {
       $from_input = explode('/',$form_state['filters']['from']);
       $fromdate = $from_input[2].'-'.$from_input[1].'-'.$from_input[0];
       $query->condition('s.modified_date',$fromdate, '>=');
   } 
   if (isset($form_state['filters']['to'])&& $form_state['filters']['to']!="") {
       $to_input = explode('/',$form_state['filters']['to']);
       $todate = $to_input[2].'-'.$to_input[1].'-'.$to_input[0];
       $query->condition('s.modified_date',$todate, '<=');
   } 
   if (isset($form_state['filters']['applicant_name'])&& $form_state['filters']['applicant_name']!="") {
       //$db_or = db_or();
       //$db_or->condition('i.firstname', '%'.db_like($form_state['filters']['applicant_name']), 'LIKE');
       $query->condition('i.name', '%'. db_like($form_state['filters']['applicant_name']) .'%', 'LIKE');
      // $query->condition($db_or);
   }
   if (isset($form_state['filters']['candidatelocation'])&& $form_state['filters']['candidatelocation']!="") {    
       $query->condition('s.country',$form_state['filters']['candidatelocation'], '=');
   } 
   if (isset($form_state['filters']['candidatestate']) && $form_state['filters']['candidatestate']!="") {   
       $query->condition('s.state',$form_state['filters']['candidatestate'], '=');
   } 
   if (isset($form_state['filters']['min'])&& $form_state['filters']['min']!="") {
       $age=$form_state['filters']['min'];
       $min_age = date('Y-m-d', strtotime("today -$age years"));
       $query->condition('s.dob', $min_age, '>=');
   }
   if (isset($form_state['filters']['max']) && $form_state['filters']['max']!="") {
       $age=$form_state['filters']['max'];
       $max_age = date('Y-m-d', strtotime("today -$age years"));
       $query->condition('s.dob', $max_age, '<=');
   }
   if (isset($form_state['filters']['highest_degree'])&& $form_state['filters']['highest_degree']!="") {    
       $query->condition('s.highest_qualification',$form_state['filters']['highest_degree'], '=');
   } 
   if (isset($form_state['filters']['status'])&& $form_state['filters']['status']!="") {    
       $query->condition('s.status',$form_state['filters']['status'], '=');
   } 
   if (isset($form_state['filters']['year_of_experience_from'])&& $form_state['filters']['year_of_experience_from']!="") {  
       $query->condition('s.experience_in_years', $form_state['filters']['year_of_experience_from'], '>=');  
   } 
   if (isset($form_state['filters']['year_of_experience_to'])&& $form_state['filters']['year_of_experience_to']!="") {  
       $query->condition('s.experience_in_years', $form_state['filters']['year_of_experience_to'], '<=');  
   } 
   
   $query->condition('s.status',3, '!=');
   $query->condition('s.eid',$user->uid, '=');
   
   $table_sort = $query->extend('TableSort') // Add table sort extender.
                       ->orderByHeader($header); // Add order by headers.

   $pager = $table_sort->extend('PagerDefault')
                       ->orderBy('s.id', 'DESC')
                       ->limit(10); // 10 rows per page.

    $result = $pager->execute();
    //print strtr((string) $query, $query->arguments());
   
    $rows = array();
    $location="";
    $employer_student_status=employer_student_status();
    foreach($result as $res):
      $name=$res->name;
      $location=get_country_name($res->country)." ".get_state_name($res->state);
        $checkboxes_display='<input type="checkbox" name="shortlist_checkboxes[]" value="'.encrypt($res->id,variable_get('IIUM_Encryption_Key', NULL)).'">';
      if($res->status==0)
       $shortlist_link='<a href="?shortlistid='.encrypt($res->id,variable_get('IIUM_Encryption_Key', NULL)).'" onclick="return shortlist();"  title="SHORTLIST" alt="Shortlist">Shortlist</a>';   
      else
       $shortlist_link='<span>Shortlist</span>';
      
      if($res->status!=2)
       $reject_link='<a href="?rejected='.encrypt($res->id,variable_get('IIUM_Encryption_Key', NULL)).'" onclick="return shortlist();"  title="SHORTLIST" alt="Shortlist">Reject</a>';   
      else
       $reject_link='<span>Reject</span>';
           
      $experience=$res->experience_in_years." Years ";
      if($res->experience_in_months>0)
       $experience.=$res->experience_in_months." Months";
      
      $rows[] = array($checkboxes_display,$res->uid,$name,$experience,$location,$res->jobid,$res->job_title,date("d-m-Y",strtotime($res->modified_date)),$employer_student_status[$res->status],'<div class="options"><a href="viewresume/'.encrypt($res->sjid,variable_get('IIUM_Encryption_Key', NULL)).'/'.encrypt($res->uid,variable_get('IIUM_Encryption_Key', NULL)).'" class="popup" alt="VIEW" title="VIEW">View CV</a> '.$shortlist_link.$reject_link.'</div>');   
    endforeach;
        
    
    $form['table'] = array(
      '#theme' => 'table',
      '#header' => $header,
      '#rows' => $rows,
      '#empty' => t('No Records Found!')
    );
    
  $form['pager'] = array('#markup' => theme('pager'));
  
    return $form;
    
}
function levelofqualifcation()
{
    $qualifcation = db_select('qualifcation')
                  ->fields('qualifcation')
                  ->execute()
                  ->fetchAll();
        $qualifcation_arr[''] = 'Select';
        foreach ($qualifcation as $value):
            $qualifcation_arr[$value->id] = $value->name;
        endforeach;
    return $qualifcation_arr;     
}