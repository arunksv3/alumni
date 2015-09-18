<?php
/**
 * Function for handling ajax request
 */
function employer_viewstate_list_ajax($form, $form_state)
{
    //echo $form['filter']['wrapper'];  
   return $form['filter']['wrapper'];  
}

function employer_shortlitedapplicants_submit($form, &$form_state) {
      $form_state['filters']['job_id'] = $form_state['values']['job_id']; 
      $form_state['filters']['job_title'] = $form_state['values']['job_title'];
      $form_state['filters']['applicant_name'] = $form_state['values']['applicant_name']; 
      $form_state['filters']['candidatelocation'] = $form_state['values']['candidatelocation']; 
      $form_state['filters']['candidatestate'] = isset($form_state['values']['candidatestate'])?$form_state['values']['candidatestate']:''; 
      $form_state['filters']['status'] = $form_state['values']['status']; 
      $form_state['filters']['employer_action'] = $form_state['values']['employer_action']; 
      $form_state['rebuild'] = TRUE;
    }
/*View applicants function*/
function employer_shortlistedapplicants($form, &$form_state)
{  
   global $user;     
    
   if(isset($_GET) && isset($_GET['selected']) && $_GET['selected']!=""){
          $fields = array(
            'status' => 4,
           );
          /*Decrypting the data using configration key*/
            $decrypting_key=variable_get('IIUM_Encryption_Key', NULL);
            $shortlistid=(int)decrypt($_GET['selected'],$decrypting_key);
            
          $employer_jobs_delete=db_update('student_job_details')
                         ->fields($fields)
                         ->condition('id',$shortlistid)
                         ->execute(); 
       //call studentid jobid function
       $job_studentid_jobid=job_studentid_jobid($shortlistid);
       $user_status_history=user_status_history($user->uid,$job_studentid_jobid->uid,$job_studentid_jobid->sjid,4,'employer');
      audit_log('selected', 'user id: '.$job_studentid_jobid->uid."<br> job id: ".$job_studentid_jobid->sjid); 
      if($user_status_history==TRUE)
       { 
        drupal_set_message('Applicant is successfully selected for this Job.', 'status');
        drupal_goto('employer/shortlistedapplicants');
       }  
     }
    if(isset($_GET) && isset($_GET['rejected']) && $_GET['rejected']!=""){
          $fields = array(
            'status' => 2,
           );
          /*Decrypting the data using configration key*/
            $decrypting_key=variable_get('IIUM_Encryption_Key', NULL);
            $shortlistid=(int)decrypt($_GET['rejected'],$decrypting_key);
            
          $employer_jobs_delete=db_update('student_job_details')
                         ->fields($fields)
                         ->condition('id',$shortlistid)
                         ->execute(); 
        //call studentid jobid function
        $job_studentid_jobid=job_studentid_jobid($shortlistid);
        $user_status_history=user_status_history($user->uid,$job_studentid_jobid->uid,$job_studentid_jobid->sjid,2,'employer');  
       audit_log('rejected', 'user id: '.$job_studentid_jobid->uid."<br> job id: ".$job_studentid_jobid->sjid); 
 
      if($user_status_history==TRUE)
       { 
        drupal_set_message('Selected Applicant is Rejected. Applicant record will be removed from shortlist, You will find this record back in "View applicants" tab.', 'status');
        drupal_goto('employer/shortlistedapplicants');
       }  
     }
     drupal_add_css(drupal_get_path('theme', 'bootstrap_business') .'/css/jquery-ui.css');  
     drupal_add_js(drupal_get_path('theme', 'bootstrap_business') .'/js/jquery-ui.js');
     drupal_add_js('jQuery(document).ready(function($) { 
        $("#from").datepicker({
        changeMonth: true,
        changeYear: true,
        showOn: "both",
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
        function selected(){
        var cnf = confirm('Are you sure you want to select this applicant for this Job? Please click Ok to proceed.');
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
    $form['filter']['applicant_name'] = array(
      '#type' => 'textfield',
      '#title' => t('Applicant Name'),
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
    $form['filter']['status'] = array(
    '#type' => 'select', 
    '#title' => 'Interview Request Status',
    '#options' => array('' => 'All')+employer_student_shortlisted_status(),
    );
    $form['filter']['employer_action'] = array(
    '#type' => 'select', 
    '#title' => 'Employer Action',
    '#options' => array('' => 'All')+employer_actions(),
    );
    $form['filter']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Search'),
      '#submit' => array('employer_shortlitedapplicants_submit'),
    );

   $header = array(
             array('data' => t('Applicant ID'), 'width' => '10%'), 
             array('data' => t('Applicant Name'), 'width' => '12%'),
             array('data' => t('Location'), 'width' => '10%'),
             array('data' => t('Job ID'), 'width' => '10%'),
             array('data' => t('Job Title'), 'width' => '20%'),
             array('data' => t('Request Sent On'), 'width' => '8%'),
             array('data' => t('Interview Request Status'), 'width' => '18%'),
             array('data' => t('Options'), 'width' => '12%'),
           );
   $query = db_select('student_job_details', 's');
   $query->innerjoin('employer_jobs', 'e', 'e.id = s.sjid');
   $query->leftJoin('student_interview_request', 'si', 'si.student_id = s.uid and si.jobid = s.sjid');
   $query->leftJoin('stdinfo', 'i', 's.uid = i.uid');
   
   $query->fields('s', array('id','uid','firstname','lastname','sjid','country','state','status','created_date'));
   $query->fields('e', array('jobid','job_title'));
   $query->fields('si', array('status'));
    $query->fields('i', array('name','alt_email'));

   if (isset($form_state['filters']['job_id']) && $form_state['filters']['job_id']!="") {
       $query->condition('e.jobid', '%' . db_like($form_state['filters']['job_id']) . '%', 'LIKE');
   }
   if (isset($form_state['filters']['job_title']) && $form_state['filters']['job_title']!="") {
       $query->condition('e.job_title', '%' . db_like($form_state['filters']['job_title']) . '%', 'LIKE');
   }
   if (isset($form_state['filters']['applicant_name'])&& $form_state['filters']['applicant_name']!="") {
       //$db_or = db_or();
       //$db_or->condition('s.firstname', '%'.db_like($form_state['filters']['applicant_name']), 'LIKE');
       $query->condition('i.name', '%'. db_like($form_state['filters']['applicant_name']) .'%', 'LIKE');
       //$query->condition($db_or);
   }
   if (isset($form_state['filters']['candidatelocation'])&& $form_state['filters']['candidatelocation']!="") {    
       $query->condition('s.country',$form_state['filters']['candidatelocation'], '=');
   } 
   if (isset($form_state['filters']['candidatestate']) && $form_state['filters']['candidatestate']!="") {   
       $query->condition('s.state',$form_state['filters']['candidatestate'], '=');
   }   
   if (isset($form_state['filters']['status'])&& $form_state['filters']['status']!="") {    
       $query->condition('si.status',$form_state['filters']['status'], '=');
   } 
   if (isset($form_state['filters']['employer_action'])&& $form_state['filters']['employer_action']!="") {    
       $query->condition('s.status',$form_state['filters']['employer_action'], '=');
   }
   
   $db_or_status= db_or();
   $db_or_status->condition('s.status',1, '='); 
   $db_or_status->condition('s.status',4, '=');
   $query->condition($db_or_status);
   
   $query->condition('s.eid',$user->uid, '=');
   $table_sort = $query->extend('TableSort') // Add table sort extender.
                       ->orderByHeader($header); // Add order by headers.

   $pager = $table_sort->extend('PagerDefault')
                       ->orderBy('s.id', 'DESC')
                       ->limit(10); // 10 rows per page.

    $result = $pager->execute();
    //->fetchAll(PDO::FETCH_ASSOC);
    //print strtr((string) $query, $query->arguments());
        
    $employer_student_shortlisted_status=employer_student_shortlisted_status();
    $employer_student_status_function=employer_student_status();
    $rows = array();
    $location="";
    foreach($result as $res):
      $name=$res->name;
      $location=get_country_name($res->country)." ".get_state_name($res->state); 
      $view_interview_request='<a href="viewresume/'.encrypt($res->sjid,variable_get('IIUM_Encryption_Key', NULL)).'/'.encrypt($res->uid,variable_get('IIUM_Encryption_Key', NULL)).'" class="popup" alt="VIEW" title="VIEW">View CV</a>';
      if($res->si_status!="")
      {
        $interview_status=$employer_student_shortlisted_status[$res->si_status];
        $view_interview_request.="<span>Send Interview Request</span>";
      }
     else 
     {
      $interview_status="No Request Sent";
      if($res->status==4)
        $view_interview_request.="<span>Send Interview Request</span>";  
      else
        $view_interview_request.='<a href="interviewrequest/'.encrypt($res->sjid,variable_get('IIUM_Encryption_Key', NULL)).'/'.encrypt($res->uid,variable_get('IIUM_Encryption_Key', NULL)).'" class="popup" alt="VIEW" title="VIEW">Send Interview Request</a>'; 
      
     }
      if($res->status==4) 
        $view_interview_request.='<span>Reject Applicant</span> <span>Select Applicant</span>';
      else 
        $view_interview_request.='<a href="?rejected='.encrypt($res->id,variable_get('IIUM_Encryption_Key', NULL)).'" onclick="return rejected();">Reject Applicant</a> <a href="?selected='.encrypt($res->id,variable_get('IIUM_Encryption_Key', NULL)).'" onclick="return selected();">Select Applicant</a>';
     
      $employer_student_status='<span class="final-status">Employer Action:'.$employer_student_status_function[$res->status].'</span>';
      
      $rows[] = array($res->uid,$name,$location,$res->jobid,$res->job_title,date("d-m-Y", strtotime($res->created_date)),$interview_status,'<div class="options">'.$view_interview_request.$employer_student_status.'</div>');   
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
