<?php
/**
 * Student Summary Report Function.
 */
function ajax_qualification_detail_callback($form, $form_state) {
    return $form['filter']['wrapper']['highest']['qualification_level'];
}
function detail_report_select_ajax($form, $form_state) {
    return $form['filter']['wrapper'];
}
function detail_reports_submit($form, &$form_state) {  
   $form_state['rebuild'] = TRUE;
}
function detail_list_state_ajax($form, $form_state)
{
  // Return the dropdown list including the wrapper
  return $form['filter']['wrapper']['job_country_prefered']['wrapper'];   
}
function detail_reports($form, &$form_state) { 
     $display="";
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
        });', 'inline');
    $form = array();
    $form['filter']['daterange'] = array(
      '#type' => 'fieldset',
      '#title' => t('Date Range'),
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
    $form['filter']['summary_report_select'] = array(
    '#type' => 'select',
    '#title' => t('Report Type'),
    '#options' => array(''=>'Select Report Type',1=>'No. of employers registered',2=>'No. of employers advertise job',3=>'No. of employers call for interview'
          ,4=>'Employers Preferences by'),
    '#ajax' => array(
       'event'=>'change',
       'callback' => 'detail_report_select_ajax',
       'wrapper' => 'subparrent-list-wrapper',
     ),
    );
  $form['filter']['wrapper'] = array(
     '#prefix' => '<div id="subparrent-list-wrapper" class="employer-detail-report">',
     '#suffix' => '</div>',
    );
  
  if(isset($_GET['summary_report_select']) && $_GET['summary_report_select']!='')
   $form_state['values']['summary_report_select']=$_GET['summary_report_select'];
  
  if($form_state['values']['summary_report_select']==1)
   {
    $form['filter']['wrapper']['employer'] = array(
    '#type' => 'select',
    '#options' => all_employers(),
    '#title' => 'Company Name',
    );
     $form['filter']['wrapper']['job_country'] = array(
        '#type' => 'select',
        '#title' => t('Country'),
        '#options' => array(''=>'Select') + countries_list(),
        '#ajax' => array(
            'event' => 'change',
            'callback' => 'detail_list_state_ajax',
            'wrapper' => 'state-list-wrapper',
        ),
       );
        // Wrapper for state dropdown list
      $form['filter']['wrapper']['job_country_prefered']['wrapper'] = array(
          '#prefix' => '<div id="state-list-wrapper">',
          '#suffix' => '</div>',
       );
       if ((isset($form_state['values']['job_country']) && $form_state['values']['job_country'] != ""))
       {
           $cid = $form_state['values']['job_country'];
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
            $form['filter']['wrapper']['job_country_prefered']['wrapper']['job_state'] = array(
                '#type' => 'select',
                '#title' => t('State'),
                '#options' => $state_list_arr,
            );
        }
       }
    $form['filter']['wrapper']['industry'] = array(
    '#type' => 'select',
    '#options' => array(''=>'select')+nature_bussiness_arr(),
    '#title' => 'Industry',
    );
    $form['filter']['wrapper']['status'] = array(
    '#type' => 'select',
    '#options' => array(''=>'select status',1=>'Active',0=>'Inactive'),
    '#title' => 'Status',
    );
    $form['filter']['wrapper']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Submit'),
      '#submit' => array('detail_reports_submit'),
    );
     $form['filter']['wrapper']['print_report'] = array(
      '#type' => 'fieldset',
      '#title' => t('<span><a href="javascript:window.print();">Print Report</a>&nbsp;|&nbsp;<a href="?pdf=pdf&summary_report_select=1">Download PDF</a></span>'), 
    );
     $header = array(
             array('data' => t('Company Name'), 'width' => '6%'), 
             array('data' => t('Email'), 'width' => '17%'),
             array('data' => t('Location'), 'width' => '17%'),
             array('data' => t('Industry'), 'width' => '9%'),
             array('data' => t('status'), 'width' => '8%'),
             array('data' => t('Register Date'), 'width' => '8%'),     
           );
    
       $results=db_select('employer_registration', 'r'); 
       $results->innerjoin('users', 'u', 'u.uid = r.uid');
       $results->fields('r',array('company_organization','nature_bussiness','country','state','created_date'));
       $results->fields('u',array('mail','status'));

       if(isset($form_state['values']['from'])&& $form_state['values']['from']!="")
        {
          $from_input = explode('/',$form_state['values']['from']);
          $fromdate = $from_input[2].'-'.$from_input[1].'-'.$from_input[0];
          $results->condition('r.created_date',$fromdate,'>=');
        }
       if(isset($form_state['values']['to'])&& $form_state['values']['to']!="")
        {
          $to_input = explode('/',$form_state['values']['to']);
          $todate = $to_input[2].'-'.$to_input[1].'-'.$to_input[0];   
          $results->condition('r.created_date',$todate,'<=');
        }
       if(isset($form_state['values']['employer'])&& $form_state['values']['employer']!="")
        {
          $results->condition('r.uid',$form_state['values']['employer'],'=');
        }
       if(isset($form_state['values']['job_country'])&& $form_state['values']['job_country']!="")
        {
          $results->condition('r.country',$form_state['values']['job_country'],'=');
        } 
       if(isset($form_state['values']['job_state'])&& $form_state['values']['job_state']!="")
        {
          $results->condition('r.state',$form_state['values']['job_state'],'=');
        }  
       if(isset($form_state['values']['industry'])&& $form_state['values']['industry']!="")
        {
           $child_db_or = db_or();
           $child_db_or->condition('r.nature_bussiness',db_like($form_state['values']['industry']).'|%','LIKE');
           $child_db_or->condition('r.nature_bussiness',$form_state['values']['industry'],'=');
           $child_db_or->condition('r.nature_bussiness','%|' .db_like($form_state['values']['industry']).'|%','LIKE');
           $child_db_or->condition('r.nature_bussiness','%|' .db_like($form_state['values']['industry']),'LIKE');
           $results->condition($child_db_or);
        }  
        if(isset($form_state['values']['status'])&& $form_state['values']['status']!="")
        {
          $results->condition('u.status',$form_state['values']['status'],'=');
        }   
       $table_sort = $results->extend('TableSort') // Add table sort extender.
                             ->orderByHeader($header); // Add order by headers.

       $pager = $table_sort->extend('PagerDefault')
                           ->orderBy('r.eid', 'DESC');
       if(isset($_GET['pdf']) && $_GET['pdf']=='pdf')
         $pager->limit(200000000); 
       else
         $pager->limit(10); //10 rows per page.

      $result = $pager->execute();     
      $nature_bussiness_arr=nature_bussiness_arr();
      $pdf_employer_records='<tr><th>Company Name</th><th>Email</th><th>Location</th><th>Industry</th><th>Status</th><th>Register Date</th></tr>';
      foreach($result as $res):
        $nature_bussiness_all="";
        $nature_bussiness=explode("|",$res->nature_bussiness);
         foreach($nature_bussiness as $nature_bussiness_ind):
          if($nature_bussiness_ind!="")
          $nature_bussiness_all.=$nature_bussiness_arr[$nature_bussiness_ind]."<br>";
         endforeach;   
       $created_date=date('d-m-Y',strtotime($res->created_date));
       if($res->status==1)
        $status='Active'; 
       else 
        $status='Inactive';     
       
       $rows[] =array($res->company_organization,$res->mail,get_country_name($res->country)."<br>".get_state_name($res->state),$nature_bussiness_all,$status,$created_date);
       $pdf_employer_records.="<tr><td>".$res->company_organization."</td><td>".$res->mail."</td><td>".get_country_name($res->country)."<br>".get_state_name($res->state)."</td><td>".$nature_bussiness_all."</td><td>".$status."</td><td>".$created_date."</td></tr>"; 
     endforeach;  
   
     $_SESSION['pdf_employer_records']=$pdf_employer_records;
     $form['filter']['wrapper']['table'] = array(
      '#theme' => 'table',
      '#header' => $header,
      '#rows' => $rows,
      '#empty' => t('No Records Found!')
    );
    
    $form['filter']['wrapper']['pager'] = array('#markup' => theme('pager'));
   }
   else if($form_state['values']['summary_report_select']==2)
   {
    $form['filter']['wrapper']['employer'] = array(
    '#type' => 'select',
    '#options' => all_employers(),
    '#title' => 'Company Name',
    );
    $form['filter']['wrapper']['positionapplied'] = array(
    '#type' => 'select',
    '#options' => positionoffered(),
    '#title' => 'Hiring Position',
    );
    $form['filter']['wrapper']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Submit'),
      '#submit' => array('detail_reports_submit'),
    );
   $form['filter']['wrapper']['print_report'] = array(
      '#type' => 'fieldset',
      '#title' => t('<span><a href="javascript:window.print();">Print Report</a>&nbsp;|&nbsp;<a href="?pdf=pdf&summary_report_select=2">Download PDF</a></span>'), 
    ); 
     $header = array(
             array('data' => t('Company Name'), 'width' => '6%'), 
             array('data' => t('Job ID'), 'width' => '17%'),
             array('data' => t('Job Title'), 'width' => '17%'),
             array('data' => t('Hiring Position'), 'width' => '17%'),
             array('data' => t('Location'), 'width' => '17%'),
             array('data' => t('Job Posted Date'), 'width' => '8%'),         
           );
       $pdf_employer_records='<tr><th>Company Name</th><th>Job ID</th><th>Job Title</th><th>Hiring Position</th><th>Job Posted Date</th></tr>';
       $results=db_select('employer_jobs', 'e'); 
       $results->innerjoin('employer_registration', 'r', 'r.uid = e.uid');
       $results->fields('r',array('company_organization'));
       $results->fields('e',array('position_offered','jobid','job_title','date_time','job_country','job_state'));
      
       if(isset($form_state['values']['from'])&& $form_state['values']['from']!="")
        {
          $from_input = explode('/',$form_state['values']['from']);
          $fromdate = $from_input[2].'-'.$from_input[1].'-'.$from_input[0];
          $results->condition('e.date_time',$fromdate,'>=');
        }
       if(isset($form_state['values']['to'])&& $form_state['values']['to']!="")
        {
          $to_input = explode('/',$form_state['values']['to']);
          $todate = $to_input[2].'-'.$to_input[1].'-'.$to_input[0];   
          $results->condition('e.date_time',$todate,'<=');
        }
        if(isset($form_state['values']['employer'])&& $form_state['values']['employer']!="")
        {
          $results->condition('r.uid',$form_state['values']['employer'],'=');
        }
        if(isset($form_state['values']['positionapplied'])&& $form_state['values']['positionapplied']!="")
        {
          $results->condition('e.position_offered',$form_state['values']['positionapplied'],'=');
        }
      $table_sort = $results->extend('TableSort') // Add table sort extender.
                      ->orderByHeader($header); // Add order by headers.

      $pager = $table_sort->extend('PagerDefault')
                       ->orderBy('r.eid', 'DESC');
       if(isset($_GET['pdf']) && $_GET['pdf']=='pdf')
         $pager->limit(200000000); 
       else
         $pager->limit(10); //10 rows per page.

      $result = $pager->execute();
      $positionoffered=positionoffered();
      foreach($result as $res):  
        $created_date=date('d-m-Y',strtotime($res->date_time));      
        $rows[] =array($res->company_organization,$res->jobid,$res->job_title,$positionoffered[$res->position_offered],get_country_name($res->job_country)."<br>".get_state_name($res->job_state),$created_date);
        $pdf_employer_records.="<tr><td>".$res->company_organization."</td><td>".$res->jobid."</td><td>".$res->job_title."</td><td>".$positionoffered[$res->position_offered]."</td><td>".get_country_name($res->country)."<br>".get_state_name($res->state)."</td><td>".$created_date."</td></tr>"; 
      endforeach;  
      $_SESSION['pdf_employer_records']=$pdf_employer_records;
     $form['filter']['wrapper']['table'] = array(
      '#theme' => 'table',
      '#header' => $header,
      '#rows' => $rows,
      '#empty' => t('No Records Found!')
    );
    
    $form['filter']['wrapper']['pager'] = array('#markup' => theme('pager'));
    
    
   }
   else if($form_state['values']['summary_report_select']==3)
   {
    $form['filter']['wrapper']['employer'] = array(
    '#type' => 'select',
    '#options' => all_employers(),
    '#title' => 'Company Name',
    );
    $form['filter']['wrapper']['positionapplied'] = array(
    '#type' => 'select',
    '#options' => positionoffered(),
    '#title' => 'Position',
    );
    $form['filter']['wrapper']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Submit'),
      '#submit' => array('detail_reports_submit'),
    );
    $form['filter']['wrapper']['print_report'] = array(
      '#type' => 'fieldset',
      '#title' => t('<span><a href="javascript:window.print();">Print Report</a>&nbsp;|&nbsp;<a href="?pdf=pdf&summary_report_select=2">Download PDF</a></span>'), 
    );
     $header = array(
             array('data' => t('Company Name'), 'width' => '6%'), 
             array('data' => t('Job ID'), 'width' => '17%'),
             array('data' => t('Job Title'), 'width' => '17%'),
             array('data' => t('Hiring Position'), 'width' => '17%'),
             array('data' => t('Location'), 'width' => '17%'),
             array('data' => t('Call for interview Date'), 'width' => '8%'),         
           );
        $pdf_employer_records='<tr><th>Company Name</th><th>Job ID</th><th>Job Title</th><th>Hiring Position</th><th>Call for interview Date</th></tr>';
         $results=db_select('student_interview_request', 's'); 
         $results->innerjoin('employer_jobs', 'e', 'e.uid = s.employer_id');
         $results->innerjoin('employer_registration', 'r', 'r.uid = e.uid');
         $results->fields('r',array('company_organization'));
         $results->fields('e',array('position_offered','jobid','job_title','job_country','job_state'));
         $results->fields('s',array('created_date'));
           
       if(isset($form_state['values']['from'])&& $form_state['values']['from']!="")
        {
          $from_input = explode('/',$form_state['values']['from']);
          $fromdate = $from_input[2].'-'.$from_input[1].'-'.$from_input[0];
          $results->condition('s.created_date',$fromdate,'>=');
        }
       if(isset($form_state['values']['to'])&& $form_state['values']['to']!="")
        {
          $to_input = explode('/',$form_state['values']['to']);
          $todate = $to_input[2].'-'.$to_input[1].'-'.$to_input[0];   
          $results->condition('s.created_date',$todate,'<=');
        }
        if(isset($form_state['values']['employer'])&& $form_state['values']['employer']!="")
        {
          $results->condition('r.uid',$form_state['values']['employer'],'=');
        }
        if(isset($form_state['values']['positionapplied'])&& $form_state['values']['positionapplied']!="")
        {
          $results->condition('e.position_offered',$form_state['values']['positionapplied'],'=');
        }
      $table_sort = $results->extend('TableSort') // Add table sort extender.
                      ->orderByHeader($header); // Add order by headers.

      $pager = $table_sort->extend('PagerDefault')
                       ->orderBy('r.eid', 'DESC');
       if(isset($_GET['pdf']) && $_GET['pdf']=='pdf')
         $pager->limit(200000000); 
       else
         $pager->limit(10); //10 rows per page.

      $result = $pager->execute();
      $positionoffered=positionoffered();
      foreach($result as $res):
        $created_date=date('d-m-Y',strtotime($res->created_date));      
        $rows[] =array($res->company_organization,$res->jobid,$res->job_title,$positionoffered[$res->position_offered],get_country_name($res->job_country)."<br>".get_state_name($res->job_state),$created_date);  
        $pdf_employer_records.="<tr><td>".$res->company_organization."</td><td>".$res->jobid."</td><td>".$res->job_title."</td><td>".$positionoffered[$res->position_offered]."</td><td>".get_country_name($res->country)."<br>".get_state_name($res->state)."</td><td>".$created_date."</td></tr>"; 
      endforeach;  
     $_SESSION['pdf_employer_records']=$pdf_employer_records; 
     $form['filter']['wrapper']['table'] = array(
      '#theme' => 'table',
      '#header' => $header,
      '#rows' => $rows,
      '#empty' => t('No Records Found!')
    );
    
    $form['filter']['wrapper']['pager'] = array('#markup' => theme('pager'));
   }
   else if($form_state['values']['summary_report_select']==4)
   {
    $form['filter']['wrapper']['employer'] = array(
    '#type' => 'select',
    '#options' => all_employers(),
    '#title' => 'Company Name',
    );
    $form['filter']['wrapper']['positionapplied'] = array(
    '#type' => 'select',
    '#options' => positionoffered(),
    '#title' => 'Position',
    );
    $form['filter']['wrapper']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Submit'),
      '#submit' => array('detail_reports_submit'),
    );
    $form['filter']['wrapper']['print_report'] = array(
      '#type' => 'fieldset',
      '#title' => t('<span><a href="javascript:window.print();">Print Report</a>&nbsp;|&nbsp;<a href="?pdf=pdf">Download PDF</a></span>'), 
    );
       $header = array(
             array('data' => t('Company Name'), 'width' => '6%'), 
             array('data' => t('Job ID'), 'width' => '17%'),
             array('data' => t('Job Title'), 'width' => '17%'),
             array('data' => t('Hiring Position'), 'width' => '17%'),
             array('data' => t('Location'), 'width' => '17%'),
             array('data' => t('Job Posted Date'), 'width' => '8%'),         
           );
    
       $results=db_select('employer_jobs', 'e'); 
       $results->innerjoin('employer_registration', 'r', 'r.uid = e.uid');
       $results->fields('r',array('company_organization'));
       $results->fields('e',array('position_offered','jobid','job_title','date_time','job_country','job_state'));
      
       if(isset($form_state['values']['from'])&& $form_state['values']['from']!="")
        {
          $from_input = explode('/',$form_state['values']['from']);
          $fromdate = $from_input[2].'-'.$from_input[1].'-'.$from_input[0];
          $results->condition('e.date_time',$fromdate,'>=');
        }
       if(isset($form_state['values']['to'])&& $form_state['values']['to']!="")
        {
          $to_input = explode('/',$form_state['values']['to']);
          $todate = $to_input[2].'-'.$to_input[1].'-'.$to_input[0];   
          $results->condition('e.date_time',$todate,'<=');
        }
        if(isset($form_state['values']['employer'])&& $form_state['values']['employer']!="")
        {
          $results->condition('r.uid',$form_state['values']['employer'],'=');
        }
        if(isset($form_state['values']['positionapplied'])&& $form_state['values']['positionapplied']!="")
        {
          $results->condition('e.position_offered',$form_state['values']['positionapplied'],'=');
        }
      $table_sort = $results->extend('TableSort') // Add table sort extender.
                      ->orderByHeader($header); // Add order by headers.

      $pager = $table_sort->extend('PagerDefault')
                       ->orderBy('r.eid', 'DESC')
                       ->limit(10); // 10 rows per page.

      $result = $pager->execute();
      $positionoffered=positionoffered();
      foreach($result as $res):  
        $created_date=date('d-m-Y',strtotime($res->date_time));      
        $rows[] =array($res->company_organization,$res->jobid,$res->job_title,$positionoffered[$res->position_offered],get_country_name($res->job_country)."<br>".get_state_name($res->job_state),$created_date);
      endforeach;   
     $form['filter']['wrapper']['table'] = array(
      '#theme' => 'table',
      '#header' => $header,
      '#rows' => $rows,
      '#empty' => t('No Records Found!')
    );
    
   $form['filter']['wrapper']['pager'] = array('#markup' => theme('pager'));
   
    
   }
    
   if(isset($_GET['pdf']) && $_GET['pdf']=='pdf')
    {
         $markup = '<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
        <html xmlns="http://www.w3.org/1999/xhtml">
        <head>
        <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
        </head>
        <body>
        <div style="text-align:center"><table>'.$_SESSION['pdf_employer_records'].'</table></div>
        </body>
        </html>';
        require_once("sites/all/modules/print/lib/dompdf/dompdf_config.inc.php");
        $dompdf = new DOMPDF;
        $dompdf->load_html($markup);
        $dompdf->render();
        $dompdf->stream("employer_detail_report.pdf");
    }
    return $form;
}

function all_years()
{
    $year_array['']='select year';  
   for($i=1991;$i<=2015;$i++)
   {
     $year_array[$i]=$i;  
   }
   return $year_array;
}
function all_employers()
{
  $employer_registration = db_select('employer_registration')
        ->fields('employer_registration',array('uid','company_organization')) 
        ->execute()->fetchAll();     
  $employer_registration_arr[''] = 'Select';
        foreach ($employer_registration as $value):
            $employer_registration_arr[$value->uid] =$value->company_organization;
        endforeach;
  return $employer_registration_arr;  
}