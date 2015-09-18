<?php
/**
 * Student Summary Report Function.
 */
function detail_list_state_ajax($form, $form_state)
{
  // Return the dropdown list including the wrapper
  return $form['filter']['wrapper']['job_country_prefered']['wrapper'];   
}
function detail_reports_submit($form, &$form_state) {  
   $form_state['rebuild'] = TRUE;
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
        $("#closing_date").datepicker({
        changeMonth: true,
        changeYear: true,
        showOn: "both",
        dateFormat:"dd/mm/yy",
        buttonImage: "'.file_create_url('public://calendar.gif').'",
        buttonImageOnly: true,     
        });  
        $("#download").click(function() {
         $("#download_pdf").val("pdf");
         document.getElementById("detail-reports").submit();
        });
        });', 'inline');
    drupal_add_js('function submit_change(){document.getElementById("download_pdf").value=""}','inline'); 
    $form = array();
    $form['filter']['wrapper'] = array(
     '#prefix' => '<div  class="job-detail-report">',
     '#suffix' => '</div>',
    );
    $form['filter']['download_pdf'] = array(
      '#type' => 'hidden',
      '#attributes' => array(
        'id' => 'download_pdf'
      )
    );
    $form['filter']['wrapper']['daterange'] = array(
      '#type' => 'fieldset',
      '#title' => t('Date Range'),
    );
    $form['filter']['wrapper']['daterange']['from'] = array(
    '#type' => 'textfield', 
    '#id' => 'from', 
    '#title' => 'From',
    '#attributes' => array('readonly' => 'readonly'),
    );
    $form['filter']['wrapper']['daterange']['to'] = array(
    '#type' => 'textfield', 
    '#id' => 'to', 
    '#title' => 'To',
    '#attributes' => array('readonly' => 'readonly'),
    );
    $form['filter']['wrapper']['positionapplied'] = array(
    '#type' => 'select',
    '#options' => positionoffered(),
    '#title' => 'Position applied',
    );
    $form['filter']['wrapper']['job_type'] = array(
    '#type' => 'checkboxes',
    '#options' => jobtype(),
    '#title' => 'Job type',
    ); 
    $form['filter']['wrapper']['employer'] = array(
    '#type' => 'select',
    '#options' => all_employers(),
    '#title' => 'Employer',
    );
    $form['filter']['wrapper']['industry'] = array(
    '#type' => 'select',
    '#options' => array(''=>'select')+nature_bussiness_arr(),
    '#title' => 'Industry',
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
    $form['filter']['wrapper']['salary'] = array(
        '#type' => 'fieldset', 
        '#title' => t('Monthly Salary (MYR) <span>- Enter salary range you are looking for in digits, Ex: 5000 </span>'), 
        ); 

    $form['filter']['wrapper']['salary']['min_salary'] = array(             
        '#type' => 'textfield',
        '#title' => t('Min'),       
    );

    $form['filter']['wrapper']['salary']['max_salary'] = array(             
        '#type' => 'textfield',
        '#title' => t('Max'),      
    );
   $form['filter']['wrapper']['closing_date'] = array(
      '#type' => 'textfield', 
      '#id' => 'closing_date', 
      '#title' => 'Closing date',
      '#attributes' => array('readonly' => 'readonly'),
    );
    $form['filter']['wrapper']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Submit'),
      '#submit' => array('detail_reports_submit'),
      '#attributes' => array('onclick' => 'javascript:submit_change();'),
    );
   $form['filter']['print_report'] = array(
      '#type' => 'fieldset',
      '#title' => t('<span><a href="javascript:window.print();">Print Report</a>&nbsp;|&nbsp;<a id="download">Download PDF</a></span>'), 
    );
  
     $header = array(
             array('data' => t('Job ID'), 'width' => '5%'), 
             array('data' => t('Job Title'), 'width' => '10%'),
             array('data' => t('Position'), 'width' => '10%'),
             array('data' => t('Job Type'), 'width' => '10%'),
             array('data' => t('Employer'), 'width' => '5%'),
             array('data' => t('Industry'), 'width' => '35%'),
             array('data' => t('Location'), 'width' => '5%'),
             array('data' => t('Salary'), 'width' => '5%'),
             array('data' => t('Published Date'), 'width' => '10%'),      
             array('data' => t('Closing Date'), 'width' => '5%'),      
           );
    
       $results = db_select('employer_jobs','e');
       $results->innerjoin('employer_registration', 'r', 'r.uid = e.uid');
       $results->fields('e',array('jobid','job_title','position_offered','jobs_type','job_country','job_state','salary','date_time','deadline_for_application')); 
       $results->fields('r',array('company_organization','nature_bussiness'));
      
      
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
       if(isset($form_state['values']['positionapplied'])&& $form_state['values']['positionapplied']!="")
        {
          $results->condition('e.position_offered',$form_state['values']['positionapplied'],'=');
        }
       if(isset($form_state['values']['job_type'])&& $form_state['values']['job_type']!="")
        {
           $job_type = $form_state['values']['job_type'];
            foreach ($job_type as $value) {
                if($value!=""){ 
                    $db_or = db_or();
                    $db_or->condition('e.jobs_type', db_like(trim($value)).',%', 'LIKE');
                    $db_or->condition('e.jobs_type', trim($value), '=');
                    $db_or->condition('e.jobs_type', '%,' .db_like(trim($value)).',%', 'LIKE');
                    $db_or->condition('e.jobs_type', '%,' .db_like(trim($value)), 'LIKE');
                    $results->condition($db_or);  
                }
            }
        } 
       if(isset($form_state['values']['employer'])&& $form_state['values']['employer']!="")
        {
          $results->condition('e.company_organization',$form_state['values']['employer'],'=');
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
       if(isset($form_state['values']['job_country'])&& $form_state['values']['job_country']!="")
        {
          $results->condition('e.job_country',$form_state['values']['job_country'],'=');
        }
       if(isset($form_state['values']['job_state'])&& $form_state['values']['job_state']!="")
        {
          $results->condition('e.job_state',$form_state['values']['job_state'],'=');
        } 
       if(isset($form_state['values']['min_salary'])&& $form_state['values']['min_salary']!="")
        {
          $results->condition('e.salary',$form_state['values']['min_salary'],'>=');
        } 
       if(isset($form_state['values']['max_salary'])&& $form_state['values']['max_salary']!="")
        {
          $results->condition('e.salary',$form_state['values']['max_salary'],'<=');
        } 
       if(isset($form_state['values']['closing_date'])&& $form_state['values']['closing_date']!="")
        {
          $closing_date_input = explode('/',$form_state['values']['closing_date']);
          $closingdate = $closing_date_input[2].'-'.$closing_date_input[1].'-'.$closing_date_input[0];
          $results->condition('e.deadline_for_application',$closingdate,'=');
        } 
      $table_sort = $results->extend('TableSort') // Add table sort extender.
                      ->orderByHeader($header); // Add order by headers.

      $pager = $table_sort->extend('PagerDefault')
                          ->orderBy('e.id', 'DESC');
      
       if(isset($form_state['values']['download_pdf']) && $form_state['values']['download_pdf']=='pdf')
         $pager->limit(200000000); 
       else
         $pager->limit(10); //10 rows per page.
     
      $result = $pager->execute();
      $positionoffered=positionoffered();
      $nature_bussiness_arr=nature_bussiness_arr();
      $jobtype=jobtype();
      foreach($result as $res): 
        $nature_bussiness_all="";$jobtypes_all="";  
        $jobs_types=explode(",",$res->jobs_type);
         foreach($jobs_types as $type):
          $jobtypes_all.=$jobtype[$type]."<br>";
         endforeach; 
        $nature_bussiness=explode("|",$res->nature_bussiness);
         foreach($nature_bussiness as $nature_bussiness_ind):
          if($nature_bussiness_ind!="")
          $nature_bussiness_all.=$nature_bussiness_arr[$nature_bussiness_ind]."<br>";
         endforeach;
         $date_time=date("d-m-Y",strtotime($res->date_time)); 
         $deadline_for_application=date("d-m-Y",strtotime($res->deadline_for_application)); 
         
        $rows[] =array($res->jobid,$res->job_title,$positionoffered[$res->position_offered],$jobtypes_all,$res->company_organization,$nature_bussiness_all,get_country_name($res->job_country)."<br>".get_state_name($res->job_state),$res->salary.' (MYR)',$date_time,$deadline_for_application);
        $pdf_records.="<tr><td>".$res->jobid."</td><td>".$res->job_title."</td><td>".$positionoffered[$res->position_offered]."</td><td>".$jobtypes_all."</td><td>".$res->company_organization."</td><td>".$nature_bussiness_all."</td><td>".get_country_name($res->job_country)."<br>".get_state_name($res->job_state)."</td><td>".$res->salary.' (MYR)'."</td><td>".$date_time."</td><td>".$deadline_for_application."</td></tr>";
        endforeach;  
     $form['filter']['table'] = array(
      '#theme' => 'table',
      '#header' => $header,
      '#rows' => $rows,
      '#empty' => t('No Records Found!')
    );
    $form['filter']['pager'] = array('#markup' => theme('pager'));

    $_SESSION['pdf_records']=$pdf_records;
   if(isset($form_state['values']['download_pdf']) && $form_state['values']['download_pdf']=='pdf')
    {
         $markup = '<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
        <html xmlns="http://www.w3.org/1999/xhtml">
        <head>
        <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
		<style>
table.job-detail{border-collapse: collapse; border-color: #ccc; border-width: 1px; color: #333333; font-family: verdana,arial,sans-serif; font-size: 11px;}
table.job-detail tr{background-color:#e8e8e8;}
table.job-detail th{background-color:#dddddc; border-color: #ccc; border-style: solid; border-width: 1px; padding: 8px;}
table.job-detail td{border-color: #ccc; border-style: solid; border-width: 1px; padding: 8px; background:#f7f7f5; font-size:11px;}
</style>
        </head>
        <body>
        <div style="text-align:center">
		<table width="85%" border="0" cellspacing="0" cellpadding="0" class="job-detail">
		<tr>
		<th width="5%">Job ID</th>
		<th width="10%">Job Title</th>
		<th width="10%">Position</th>
		<th width="5%">Job Type</th>
		<th width="5%">Employer</th>
        <th width="25%">Industry</th>
		<th width="5%">Location</th>
		<th width="5%">Salary</th>
		<th width="10%">Published Date</th>
		<th width="5%">Closing Date</th></tr>'.$_SESSION['pdf_records'].'</table></div>
        </body>
        </html>';
        require_once("sites/all/modules/print/lib/dompdf/dompdf_config.inc.php");
        $dompdf = new DOMPDF;
        $dompdf->load_html($markup);
        $dompdf->render();
        $dompdf->stream("Job_Detail_Report.pdf");
    }

    return $form;
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