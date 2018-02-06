#include "pic_sim.h"
#include "algorithms/field_solver_integral.h"
#include "algorithms/ptc_pusher_geodesic.h"
#include "algorithms/current_deposit_Esirkepov.h"
#include <functional>
#include <memory>

namespace Aperture {

// PICSim::PICSim() :
//     PICSim(Environment::get_instance()) {}

PICSim::PICSim(Environment& env) : m_env(env) {
  // Initialize modules
  // TODO: select current deposition method according to config
  m_depositer = std::make_unique<CurrentDepositer_Esirkepov>(m_env);
  m_depositer->set_periodic(env.conf().boundary_periodic[0]);

  // TODO: select field solver according to config
  m_field_solver = std::make_unique<FieldSolver_Integral>(m_env.local_grid(),
                                                          m_env.local_grid_dual());

  // TODO: select particle mover type according to config
  // int interp_order = m_env.conf().interpolation_order;

  // Implement particle pusher
  m_pusher = std::make_unique<ParticlePusher_Geodesic>();
  m_pusher->set_periodic(env.conf().boundary_periodic[0]);

  // TODO: figure out a way to set algorithm
  // if (m_env.conf().algorithm_ptc_push == "Vay")
  //   m_pusher -> set_algorithm(ForceAlgorithm::Vay);
  // else if (m_env.conf().algorithm_ptc_push == "Boris")
  //   m_pusher -> set_algorithm(ForceAlgorithm::Boris);
  // m_pusher -> set_gravity(m_env.conf().gravity);
  // m_pusher -> set_radiation(bool radiation)

  // Register communication callbacks
  // m_depositer->register_current_callback(
  //     [this](VectorField<Scalar>& j) { m_comm->put_guard_cells(j); });

  // m_depositer->register_rho_callback(
  //     [this](ScalarField<Scalar>& rho) { m_comm->put_guard_cells(rho); });

  // m_field_solver->register_comm_callback(
  //     [this](VectorField<Scalar>& f) -> void { m_comm->get_guard_cells(f); });

  // m_field_solver->register_comm_callback(
  //     [this](ScalarField<Scalar>& f) -> void { m_comm->get_guard_cells(f); });

  // auto &comm = *m_comm;
  // std::function<void(VectorField<Scalar>&)> vcall = [&comm](VectorField<Scalar>& f) -> void { comm.get_guard_cells(f); };
  // m_field_solver->register_comm_callback(std::bind(
  //     static_cast<void(DomainCommunicator::*)(VectorField<Scalar>&)>(&DomainCommunicator::get_guard_cells), &comm));
  // m_field_solver->register_communicator(m_comm.get());

  // Register boundary condition
  // m_field_solver->set_boundary_condition(env.boundary_conditions());
}

PICSim::~PICSim() {}

void PICSim::loop(Aperture::SimData& data, uint32_t steps, uint32_t data_freq) {
  for (uint32_t n = 0; n < steps; n++) {
    // Do stuff
    step(data, n);
  }
}

void
PICSim::step(Aperture::SimData &data, uint32_t step) {
  double dt = m_env.conf().delta_t;
  // TODO: add particle logic
  m_pusher->push(data, dt);
  m_depositer->deposit(data, dt);
  m_field_solver->update_fields(data, dt);
}

}
