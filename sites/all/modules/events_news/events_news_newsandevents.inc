<?php

function events_news_newsandevents($form, &$form_state){
    global $base_url;
    global $user;
    
    $form = array(); 
    $header = array();
    $eventsQuery = db_select('events')
        ->fields('events')
        ->condition('status',1,'=');
     $table_sort = $eventsQuery->extend('TableSort') // Add table sort extender.
        ->orderByHeader($header); // Add order by headers.

   $pager = $table_sort->extend('PagerDefault')
        ->orderBy('from_date', 'DESC')
        ->limit(5); // 10 rows per page.

    $eventsResult = $pager->execute();
    
    $output = '<div class="form-item form-type-item"><ul class="news-events-list">';
    $decrypting_key=variable_get('IIUM_Encryption_Key', NULL);
       $organizedArr = organized(); 
       $i=0;
    foreach($eventsResult as $row){
        if($row->photo!='')
            $image = ' <img src="'.file_create_url($row->photo).'"/>';
        else
            $image = '<img src="'.file_create_url('public://uploads/profile_pictures/noimage.jpg').'"/>';
        $i++;
        if($i%2==0)
            $class = 'white';
        else 
            $class = 'grey';
        $id = encrypt($row->id,$decrypting_key);
        //<h3><span>Event Code</span>: '.$row->event_code.'</h3>
        //<div class="ne-id">'.sprintf("EV-%04d", $row->id).'</div>
        $output .='<li class="'.$class.'"> 
            <div class="ne-left">'.$image.'</div>
            <div class="ne-right">
            <span class="date-time">'.date('d M Y',strtotime($row->from_date)).' (Time: '.$row->from_time.') To '.date('d M Y',strtotime($row->to_date)).' (Time: '.$row->to_time.') </span>
            <h1><a href="'.$base_url.'/event/'.$id.'">'.$row->title.'</a></h1>
            
            
            
            <h4><span>Organized By</span>: '.$organizedArr[$row->organized_by].'</h4>
            
            <p>'.substr($row->description,0,200).'</p> 
              <a href="'.$base_url.'/event/'.$id.'">Read More</a>
            </div></li>'; 
    }
     $output .= '</ul></div>';
    $form['events'] = array(
        '#type' => 'item', 
        '#title' => '<h2>Events</h2> '
    );
    
    $form['events']['list'] = array(
        '#type' => 'item',         
        '#markup' => $output,         
     ); 
    $form['events']['all'] = array(
        '#type' => 'item',         
        '#markup' =>'<a href="eventslist">View All</a>',  
     );
    
    $newsQuery = db_select('news')
        ->fields('news')
        ->condition('status',1,'=');
     $table_sort = $newsQuery->extend('TableSort') // Add table sort extender.
        ->orderByHeader($header); // Add order by headers.

   $pager = $table_sort->extend('PagerDefault')
        ->orderBy('date', 'DESC')
        ->limit(5); // 10 rows per page.

    $newsResult = $pager->execute();
     
    $output = '<div class="form-item form-type-item"><ul class="news-events-list">';
    $reportedArr = reported();
    $i = 0;
    foreach($newsResult as $row){
        if($row->photo!='')
            $image = ' <img src="'.file_create_url($row->photo).'"/>';
        else
            $image = '<img src="'.file_create_url('public://uploads/profile_pictures/noimage.jpg').'"/>';
        $i++;
        if($i%2==0)
            $class = 'white';
        else 
            $class = 'grey';
        $id = encrypt($row->id,$decrypting_key);
        $output .='<li class="'.$class.'"> 
        <div class="ne-left">'.$image.'</div>
        <div class="ne-right">
        <span class="date-time">'.date('d M Y',strtotime($row->date)).'</span> 
        <h1><a href="'.$base_url.'/news/'.$id.'">'.$row->title.'</a></h1> 
       
        <h3><span>Reported By</span>: '.$reportedArr[$row->reported_by].'</h3>         
        <p>'.substr($row->description,0,200).'</p> 
         <a href="'.$base_url.'/news/'.$id.'">Read More</a>      
               </div></li> ';
    }
     $output .= '</ul></div>';
    $form['news'] = array(
        '#type' => 'item', 
        '#title' => '<h2>News</h2> '
    );
    
    $form['news']['list'] = array(
        '#type' => 'item',         
        '#markup' =>$output,  
     ); 
    
    $form['news']['all'] = array(
        '#type' => 'item',         
        '#markup' =>'<a href="newslist">View All</a>',  
     );
    
    return $form;
}