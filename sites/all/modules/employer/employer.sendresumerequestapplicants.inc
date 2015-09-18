<?php
/**
 * Function for handling ajax request
 */
function employer_viewstate_list_ajax($form, $form_state)
{
    //echo $form['filter']['wrapper'];  
   return $form['filter']['wrapper'];  
}

function employer_resumerequestapplicants_submit($form, &$form_state) {
      $form_state['filters']['applicant_name'] = $form_state['values']['applicant_name']; 
      $form_state['filters']['candidatelocation'] = $form_state['values']['candidatelocation']; 
      $form_state['filters']['candidatestate'] = isset($form_state['values']['candidatestate'])?$form_state['values']['candidatestate']:''; 
      $form_state['filters']['status'] = $form_state['values']['status']; 
      $form_state['rebuild'] = TRUE;
    }
/*View applicants function*/
function employer_sendresumerequest_applicants($form, &$form_state)
{  
   global $user;     
     drupal_add_js('jQuery(document).ready(function($) { 
        $(".popup").click(function () {                    
          window.open($(this).attr("href"), "PopupWindow", "width=600,height=600,scrollbars=yes,resizable=no");
          return false;
        });
        
        });', 'inline');
    
    $form = array();
    $form['filter'] = array(
      '#type' => 'fieldset',
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
    '#title' => 'Resume Request Status',
    '#options' => array('' => 'All')+employer_student_shortlisted_status(),
    );
    $form['filter']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Search'),
      '#submit' => array('employer_resumerequestapplicants_submit'),
    );

   $header = array(
             array('data' => t('Applicant ID'), 'width' => '12%'), 
             array('data' => t('Applicant Name'), 'width' => '22%'),
             array('data' => t('Location'), 'width' => '10%'),
             array('data' => t('Request Sent On'), 'width' => '20%'),
             array('data' => t('Resume Request Status'), 'width' => '25%'),
             array('data' => t('Options'), 'width' => '11%'),
           );
   $query = db_select('student_resume_request', 's');
   $query->innerjoin('student_details', 'd', 'd.uid = s.student_id');
   $query->leftJoin('stdinfo', 'i', 's.student_id = i.uid');
   $query->fields('s', array('id','status','created_date','student_id'));
   $query->fields('d', array('firstname','lastname','country','state'));
   $query->fields('i', array('name','alt_email'));
   if (isset($form_state['filters']['applicant_name'])&& $form_state['filters']['applicant_name']!="") {
       //$db_or = db_or();       
       $query->condition('i.name', '%'. db_like($form_state['filters']['applicant_name']) .'%', 'LIKE');
       //$query->condition($db_or);
   }
   if (isset($form_state['filters']['candidatelocation'])&& $form_state['filters']['candidatelocation']!="") {    
       $query->condition('d.country',$form_state['filters']['candidatelocation'], '=');
   } 
   if (isset($form_state['filters']['candidatestate']) && $form_state['filters']['candidatestate']!="") {   
       $query->condition('d.state',$form_state['filters']['candidatestate'], '=');
   }  
  if (isset($form_state['filters']['status'])&& $form_state['filters']['status']!="") {    
       $query->condition('s.status',$form_state['filters']['status'], '=');
   } 
   
   $table_sort = $query->extend('TableSort') // Add table sort extender.
                       ->orderByHeader($header); // Add order by headers.

   $pager = $table_sort->extend('PagerDefault')
                       ->orderBy('s.id', 'DESC')
                       ->limit(10); // 10 rows per page.

    $result = $pager->execute();
    //->fetchAll(PDO::FETCH_ASSOC);
    //print strtr((string) $query, $query->arguments());
        
    $resume_request_status=employer_student_shortlisted_status();
    $rows = array();
    $location="";
    foreach($result as $res):
      $name=$res->name;
      $location=get_country_name($res->country)." ".get_state_name($res->state); 
      $view_interview_request='<a href="latestresume/'.encrypt($res->student_id,variable_get('IIUM_Encryption_Key', NULL)).'" class="popup" alt="VIEW" title="VIEW">View CV</a>';
      if($res->status!=2 && $res->status!=0)
       $view_interview_request.='Shortlist';   
      else   
       $view_interview_request.='<a href="directshortlist/'.encrypt($res->student_id,variable_get('IIUM_Encryption_Key', NULL)).'" class="popup"  title="SHORTLIST" alt="Shortlist">Shortlist</a>';
      
      $resume_status=$resume_request_status[$res->status];     
      $rows[] = array($res->student_id,$name,$location,date("d-m-Y", strtotime($res->created_date)),$resume_status,'<div class="options">'.$view_interview_request.'</div>');   
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
