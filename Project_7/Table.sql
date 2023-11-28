create table if not exists flight_tasks (
	id serial not null primary key,
	launchX double precision not null,
	launchY double precision not null,
	boomX double precision not null,
	boomY double precision not null,
	t0 double precision not null,
	angle double precision not null,
	type integer not null default 1
);

create table if not exists elements (
	id serial not null primary key,
	fid integer,
	constraint flight_tasks_fk foreign key (fid) references flight_tasks (id),
	launchX double precision not null,
	launchY double precision not null,
	boomX double precision not null,
	boomY double precision not null,
	tS double precision not null,
	tE double precision not null
);

create table if not exists positions (
	eid integer,
	constraint elements_fk foreign key (eid) references elements (id),
	time double precision not null,
	x double precision not null,
	y double precision not null
);

insert into flight_tasks(launchX, launchY, boomX, boomY, t0, angle) values(random()*0.5, 0, 2 + random()*0.5, 0, random()*2.5, 0.1 + random()*0.9);
insert into elements(launchX, launchY, boomX, boomY, tS, tE) select j.launchX, j.launchY, j.boomX, j.boomY, j.t0, j.t0+SQRT((j.boomX-j.launchX)*2*TAN(j.angle)/9.8) from flight_tasks j;
insert into positions(eid, time, x, y) select e.id, e.tS + (e.tE-e.tS)/2, e.launchX + ((e.tE-e.tS)/2)*(e.tE-e.tS)*9.8/(2*SIN(j.angle))*COS(j.angle), (e.tE-e.tS)*9.8/(2*SIN(j.angle))*SIN(j.angle)*((e.tE-e.tS)/2)-9.8*((e.tE-e.tS)/2)*((e.tE-e.tS)/2)/2 from flight_tasks j, elements e where j.id = e.id;

select * from flight_tasks;
select * from elements;
select * from positions;

truncate table positions, elements;
drop table positions, elements;