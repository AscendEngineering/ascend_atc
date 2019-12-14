\c ascend_db

ALTER TABLE public."Drone"
  ADD FOREIGN KEY (status) REFERENCES public."Status"(id);

ALTER TABLE public."Drone"
  ADD FOREIGN KEY (battery) REFERENCES public."Battery"(id);

ALTER TABLE public."Jobs"
  ADD FOREIGN KEY (drone) REFERENCES public."Drone"(id);
